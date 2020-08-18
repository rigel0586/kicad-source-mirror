/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 2020 KiCad Developers, see AUTHORS.txt for contributors.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, you may find one here:
 * http://www.gnu.org/licenses/old-licenses/gpl-2.0.html
 * or you may search the http://www.gnu.org website for the version 2 license,
 * or you may write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 */

#include <bitmaps.h>
#include <widgets/paged_dialog.h>
#include <pcb_edit_frame.h>
#include <pcb_expr_evaluator.h>
#include <project.h>
#include <tool/tool_manager.h>
#include <drc/drc.h>
#include <panel_setup_rules.h>
#include <wx_html_report_box.h>
#include <html_messagebox.h>
#include <scintilla_tricks.h>
#include <drc/drc_rule_parser.h>


PANEL_SETUP_RULES::PANEL_SETUP_RULES( PAGED_DIALOG* aParent, PCB_EDIT_FRAME* aFrame ) :
        PANEL_SETUP_RULES_BASE( aParent->GetTreebook() ),
        m_Parent( aParent ),
        m_frame( aFrame ),
        m_scintillaTricks( nullptr )
{
    m_scintillaTricks = new SCINTILLA_TRICKS( m_textEditor, wxT( "()" ) );

    int    size = wxNORMAL_FONT->GetPointSize();
    wxFont fixedFont( size, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL );

    for( size_t i = 0; i < wxSTC_STYLE_MAX; ++i )
        m_textEditor->StyleSetFont( i, fixedFont );

    m_compileButton->SetBitmap( KiBitmap( drc_xpm ) );

    m_textEditor->Bind( wxEVT_STC_CHARADDED, &PANEL_SETUP_RULES::onScintillaCharAdded, this );
    m_textEditor->Bind( wxEVT_STC_AUTOCOMP_CHAR_DELETED, &PANEL_SETUP_RULES::onScintillaCharAdded, this );
}


PANEL_SETUP_RULES::~PANEL_SETUP_RULES( )
{
    delete m_scintillaTricks;
};


void PANEL_SETUP_RULES::onScintillaCharAdded( wxStyledTextEvent &aEvent )
{
    m_Parent->SetModified();
    m_textEditor->SearchAnchor();

    wxString rules = m_textEditor->GetText();
    int currentPos = m_textEditor->GetCurrentPos();
    int startPos = 0;

    for( int line = m_textEditor->LineFromPosition( currentPos ); line > 0; line-- )
    {
        int      lineStart = m_textEditor->PositionFromLine( line );
        wxString beginning = m_textEditor->GetTextRange( lineStart, lineStart + 10 );

        if( beginning.StartsWith( "(rule " ) )
        {
            startPos = lineStart;
            break;
        }
    }

    enum
    {
        NONE,
        STRING,
        SEXPR_OPEN,
        SEXPR_TOKEN,
        STRUCT_REF
    };

    std::stack<wxString> sexprs;
    wxString             partial;
    int                  context = NONE;
    int                  expr_context = NONE;

    for( int i = startPos; i < currentPos; ++i )
    {
        wxChar c = m_textEditor->GetCharAt( i );

        if( c == '\\' )
        {
            i++;  // skip escaped char
        }
        else if( context == STRING )
        {
            if( c == '"' )
            {
                context = NONE;
            }
            else
            {
                if( expr_context == STRING )
                {
                    if( c == '\'' )
                        expr_context = NONE;
                    else
                        partial += c;
                }
                else if( c == '\'' )
                {
                    partial = wxEmptyString;
                    expr_context = STRING;
                }
                else if( c == '.' )
                {
                    partial = wxEmptyString;
                    expr_context = STRUCT_REF;
                }
                else
                {
                    partial += c;
                }
            }
        }
        else if( c == '"' )
        {
            partial = wxEmptyString;
            context = STRING;
        }
        else if( c == '(' )
        {
            if( context == SEXPR_OPEN && !partial.IsEmpty() )
            {
                m_textEditor->AutoCompCancel();
                sexprs.push( partial );
            }

            partial = wxEmptyString;
            context = SEXPR_OPEN;
        }
        else if( c == ')' )
        {
            if( !sexprs.empty() )
                sexprs.pop();

            context = NONE;
        }
        else if( c == ' ' )
        {
            if( context == SEXPR_OPEN && !partial.IsEmpty() )
            {
                m_textEditor->AutoCompCancel();
                sexprs.push( partial );

                if( sexprs.size() && ( sexprs.top() == "constraint"
                                    || sexprs.top() == "disallow"
                                    || sexprs.top() == "layer" ) )
                {
                    partial = wxEmptyString;
                    context = SEXPR_TOKEN;
                    continue;
                }
            }

            context = NONE;
        }
        else
        {
            partial += c;
        }
    }

    wxString tokens;

    if( context == SEXPR_OPEN )
    {
        if( sexprs.empty() )
            tokens = "rule version";
        else if( sexprs.top() == "rule" )
            tokens = "condition constraint layer";
        else if( sexprs.top() == "constraint" )
            tokens = "max min opt";
    }
    else if( context == SEXPR_TOKEN )
    {
        if( sexprs.empty() )
        {
            /* badly formed grammar */
        }
        else if( sexprs.top() == "constraint" )
        {
            tokens = "annulus_width clearance disallow hole track_width";
        }
        else if( sexprs.top() == "disallow"
              || sexprs.top() == "buried_via"
              || sexprs.top() == "graphic"
              || sexprs.top() == "hole"
              || sexprs.top() == "micro_via"
              || sexprs.top() == "pad"
              || sexprs.top() == "text"
              || sexprs.top() == "track"
              || sexprs.top() == "via"
              || sexprs.top() == "zone" )
        {
            tokens = "buried_via graphic hole micro_via pad text track via zone";
        }
        else if( sexprs.top() == "layer" )
        {
            tokens = "inner outer \"x\"";
        }
    }
    else if( context == STRING && expr_context == STRUCT_REF )
    {
        if( !sexprs.empty() && sexprs.top() == "condition" )
        {
            PROPERTY_MANAGER&  propMgr = PROPERTY_MANAGER::Instance();
            std::set<wxString> propNames;

            for( const PROPERTY_MANAGER::CLASS_INFO& cls : propMgr.GetAllClasses() )
            {
                const PROPERTY_LIST& props = propMgr.GetProperties( cls.type );

                for( PROPERTY_BASE* prop : props )
                {
                    wxString ref( prop->Name() );
                    ref.Replace( " ", "_" );
                    propNames.insert( ref );
                }
            }

            for( const wxString& propName : propNames )
                tokens += " " + propName;

            PCB_EXPR_BUILTIN_FUNCTIONS& functions = PCB_EXPR_BUILTIN_FUNCTIONS::Instance();

            for( const wxString& funcSig : functions.GetSignatures() )
                tokens += " " + funcSig;
        }
    }

    if( !tokens.IsEmpty() )
        m_scintillaTricks->DoAutocomplete( partial, wxSplit( tokens, ' ' ) );
}


void PANEL_SETUP_RULES::OnCompile( wxCommandEvent& event )
{
    m_errorsReport->Clear();

    try
    {
        std::vector<DRC_RULE*> dummyRules;

        DRC_RULES_PARSER parser( m_frame->GetBoard(), m_textEditor->GetText(), _( "DRC rules" ) );

        parser.Parse( dummyRules, m_errorsReport );
    }
    catch( PARSE_ERROR& pe )
    {
        m_Parent->SetError( pe.What(), this, m_textEditor, pe.lineNumber, pe.byteIndex );
    }

    m_errorsReport->Flush();
}


void PANEL_SETUP_RULES::OnErrorLinkClicked( wxHtmlLinkEvent& event )
{
    wxString      link = event.GetLinkInfo().GetHref();
    wxArrayString parts;
    long          line = 0, offset = 0;

    wxStringSplit( link, parts, ':' );

    if( parts.size() > 1 )
    {
        parts[0].ToLong( &line );
        parts[1].ToLong( &offset );
    }

    int pos = m_textEditor->PositionFromLine( line - 1 ) + ( offset - 1 );

    m_textEditor->GotoPos( pos );

    m_textEditor->SetFocus();
}


bool PANEL_SETUP_RULES::TransferDataToWindow()
{
    wxString   rulesFilepath = m_frame->Prj().AbsolutePath( "drc-rules" );
    wxFileName rulesFile( rulesFilepath );

    if( rulesFile.FileExists() )
    {
        wxTextFile file( rulesFile.GetFullPath() );

        if( file.Open() )
        {
            for ( wxString str = file.GetFirstLine(); !file.Eof(); str = file.GetNextLine() )
            {
                ConvertSmartQuotesAndDashes( &str );
                m_textEditor->AddText( str << '\n' );
            }
        }
    }

    m_originalText = m_textEditor->GetText();

    return true;
}


bool PANEL_SETUP_RULES::TransferDataFromWindow()
{
    if( m_originalText == m_textEditor->GetText() )
        return true;

    try
    {
        std::vector<DRC_RULE*> dummyRules;

        DRC_RULES_PARSER parser( m_frame->GetBoard(), m_textEditor->GetText(), _( "DRC rules" ) );

        parser.Parse( dummyRules, m_errorsReport );
    }
    catch( PARSE_ERROR& pe )
    {
        m_Parent->SetError( pe.What(), this, m_textEditor, pe.lineNumber, pe.byteIndex );
        return false;
    }

    if( m_textEditor->SaveFile( m_frame->Prj().AbsolutePath( "drc-rules" ) ) )
    {
        m_frame->GetToolManager()->GetTool<DRC>()->LoadRules();
        return true;
    }

    return false;
}


void PANEL_SETUP_RULES::OnSyntaxHelp( wxHyperlinkEvent& aEvent )
{
    // Do not make this full sentence translatable: it contains keywords
    // Only a few titles can be traslated.
    wxString msg;
    msg << "<b>" << _( "Top-level Clauses" ) << "</b>";
    msg <<  "<pre>"
            "(version &lt;number>)\r"
            "(rule &lt;rule_name> &lt;rule_clause> ...)\r"
            "\r</pre><b>";
    msg << _( "Rule Clauses" );
    msg <<  "</b>"
            "<pre>"
            "(constraint &lt;constraint_type> ...)\r"
            "(condition \"&lt;expression>\")\r"
            "\r</pre>"
            "<b>";
    msg << _( "Constraint Types" );
    msg <<  "</b>"
            "<pre>"
            "clearance    annulus_width   track_width     hole     dissallow\r"
            "\r</pre>"
            "<b>";
    msg << _( "Item Types" );
    msg <<  "</b>"
            "<pre>"
            "track         via               zone\r"
            "pad           micro_via         text\r"
            "hole          buried_via        graphic\r"
            "\r</pre>"
            "<b>";
    msg << _( "Examples" );
    msg <<  "</b>"
            "<pre>"
            "(rule \"copper keepout\"\r"
            "   (constraint disallow track via zone)\r"
            "   (condition \"A.insideArea('zone_name')\"))\r"
            "\r"
            "(rule \"BGA neckdown\"\r"
            "   (constraint track_width (min 0.2mm) (opt 0.25mm))\r"
            "   (constraint clearance (min 0.05) (opt 0.08mm))\r"
            "   (condition \"A.insideCourtyard('U3')\"))\r"
            "\r"
            "(rule HV\r"
            "   (constraint clearance (min 1.5mm))\r"
            "   (condition \"A.netclass == 'HV'\"))\r"
            "\r"
            "(rule HV_HV\r"
            "   (constraint clearance (min \"1.5mm + 2.0mm\"))\r"
            "   (condition \"A.netclass == 'HV' && B.netclass == 'HV'\"))\r"
            "</pre>";

    HTML_MESSAGE_BOX dlg( m_parent, _( "Syntax Help" ) );
    dlg.SetDialogSizeInDU( 320, 320 );

    dlg.AddHTML_Text( msg );
    dlg.ShowModal();
}