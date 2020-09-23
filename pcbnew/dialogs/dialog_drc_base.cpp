///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 26 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "dialog_drc_base.h"

///////////////////////////////////////////////////////////////////////////

DIALOG_DRC_BASE::DIALOG_DRC_BASE( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : DIALOG_SHIM( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( -1,-1 ), wxDefaultSize );

	wxBoxSizer* m_MainSizer;
	m_MainSizer = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizerOptions;
	bSizerOptions = new wxBoxSizer( wxHORIZONTAL );

	wxBoxSizer* bSizer12;
	bSizer12 = new wxBoxSizer( wxVERTICAL );

	m_cbReportAllTrackErrors = new wxCheckBox( this, wxID_ANY, _("Report all errors for each track"), wxDefaultPosition, wxDefaultSize, 0 );
	m_cbReportAllTrackErrors->SetToolTip( _("If selected, all DRC violations for tracks will be reported.  This can be slow for complicated designs.\n\nIf unselected, only the first DRC violation will be reported for each track connection.") );

	bSizer12->Add( m_cbReportAllTrackErrors, 0, wxALL, 5 );

	m_cbReportTracksToZonesErrors = new wxCheckBox( this, wxID_ANY, _("Test tracks against zone fills (slow)"), wxDefaultPosition, wxDefaultSize, 0 );
	m_cbReportTracksToZonesErrors->SetToolTip( _("If selected, tracks will be tested against copper zones. \nIf copper zones are up to date, this test should be not needed.\n\nThis test can be *very slow* for complicated designs.") );

	bSizer12->Add( m_cbReportTracksToZonesErrors, 0, wxBOTTOM|wxLEFT, 5 );


	bSizerOptions->Add( bSizer12, 1, wxEXPAND|wxTOP|wxRIGHT|wxLEFT, 5 );

	wxBoxSizer* bSizerOptSettings;
	bSizerOptSettings = new wxBoxSizer( wxVERTICAL );

	m_cbRefillZones = new wxCheckBox( this, wxID_ANY, _("Refill all zones before performing DRC"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerOptSettings->Add( m_cbRefillZones, 0, wxALL, 5 );

	m_cbTestFootprints = new wxCheckBox( this, wxID_ANY, _("Test for parity between PCB and schematic"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerOptSettings->Add( m_cbTestFootprints, 0, wxBOTTOM|wxRIGHT|wxLEFT, 5 );


	bSizerOptions->Add( bSizerOptSettings, 1, wxEXPAND|wxTOP|wxRIGHT|wxLEFT, 5 );


	m_MainSizer->Add( bSizerOptions, 0, wxEXPAND|wxTOP|wxBOTTOM|wxLEFT, 3 );

	m_runningResultsBook = new wxSimplebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	running = new wxPanel( m_runningResultsBook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer14;
	bSizer14 = new wxBoxSizer( wxVERTICAL );

	m_runningNotebook = new wxNotebook( running, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_panelMessages = new wxPanel( m_runningNotebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer10;
	bSizer10 = new wxBoxSizer( wxVERTICAL );

	m_Messages = new wxTextCtrl( m_panelMessages, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxTE_MULTILINE|wxTE_READONLY );
	bSizer10->Add( m_Messages, 1, wxEXPAND|wxTOP|wxRIGHT|wxLEFT, 5 );

	wxBoxSizer* bGaugeMargins;
	bGaugeMargins = new wxBoxSizer( wxVERTICAL );

	m_gauge = new wxGauge( m_panelMessages, wxID_ANY, 10000, wxDefaultPosition, wxDefaultSize, wxGA_HORIZONTAL );
	m_gauge->SetValue( 0 );
	bGaugeMargins->Add( m_gauge, 0, wxALL|wxEXPAND, 5 );


	bSizer10->Add( bGaugeMargins, 0, wxEXPAND|wxRIGHT|wxLEFT, 5 );


	m_panelMessages->SetSizer( bSizer10 );
	m_panelMessages->Layout();
	bSizer10->Fit( m_panelMessages );
	m_runningNotebook->AddPage( m_panelMessages, _("Tests Running..."), true );

	bSizer14->Add( m_runningNotebook, 1, wxEXPAND | wxALL, 5 );


	running->SetSizer( bSizer14 );
	running->Layout();
	bSizer14->Fit( running );
	m_runningResultsBook->AddPage( running, _("a page"), false );
	results = new wxPanel( m_runningResultsBook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer13;
	bSizer13 = new wxBoxSizer( wxVERTICAL );

	m_Notebook = new wxNotebook( results, ID_NOTEBOOK1, wxDefaultPosition, wxDefaultSize, 0 );
	m_panelViolations = new wxPanel( m_Notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizerViolationsBox;
	bSizerViolationsBox = new wxBoxSizer( wxVERTICAL );

	bSizerViolationsBox->SetMinSize( wxSize( -1,320 ) );
	m_markerDataView = new wxDataViewCtrl( m_panelViolations, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxDV_NO_HEADER );
	m_markerDataView->SetToolTip( _("Click on items to highlight them on the board.") );

	bSizerViolationsBox->Add( m_markerDataView, 1, wxALL|wxEXPAND, 5 );


	m_panelViolations->SetSizer( bSizerViolationsBox );
	m_panelViolations->Layout();
	bSizerViolationsBox->Fit( m_panelViolations );
	m_Notebook->AddPage( m_panelViolations, _("Violations (%d)"), false );
	m_panelUnconnectedItems = new wxPanel( m_Notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizerUnconnectedBox;
	bSizerUnconnectedBox = new wxBoxSizer( wxVERTICAL );

	m_unconnectedDataView = new wxDataViewCtrl( m_panelUnconnectedItems, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxDV_NO_HEADER );
	bSizerUnconnectedBox->Add( m_unconnectedDataView, 1, wxALL|wxEXPAND, 5 );


	m_panelUnconnectedItems->SetSizer( bSizerUnconnectedBox );
	m_panelUnconnectedItems->Layout();
	bSizerUnconnectedBox->Fit( m_panelUnconnectedItems );
	m_Notebook->AddPage( m_panelUnconnectedItems, _("Unconnected Items (%d)"), false );
	m_panelFootprintWarnings = new wxPanel( m_Notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizerFootprintsBox;
	bSizerFootprintsBox = new wxBoxSizer( wxVERTICAL );

	m_footprintsDataView = new wxDataViewCtrl( m_panelFootprintWarnings, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxDV_NO_HEADER );
	bSizerFootprintsBox->Add( m_footprintsDataView, 1, wxALL|wxEXPAND, 5 );


	m_panelFootprintWarnings->SetSizer( bSizerFootprintsBox );
	m_panelFootprintWarnings->Layout();
	bSizerFootprintsBox->Fit( m_panelFootprintWarnings );
	m_Notebook->AddPage( m_panelFootprintWarnings, _("Schematic Parity (%d)"), false );

	bSizer13->Add( m_Notebook, 1, wxEXPAND|wxRIGHT|wxLEFT, 5 );


	results->SetSizer( bSizer13 );
	results->Layout();
	bSizer13->Fit( results );
	m_runningResultsBook->AddPage( results, _("a page"), true );

	m_MainSizer->Add( m_runningResultsBook, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSeveritySizer;
	bSeveritySizer = new wxBoxSizer( wxHORIZONTAL );

	m_showLabel = new wxStaticText( this, wxID_ANY, _("Show:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_showLabel->Wrap( -1 );
	bSeveritySizer->Add( m_showLabel, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxLEFT, 5 );

	m_showAll = new wxCheckBox( this, wxID_ANY, _("All"), wxDefaultPosition, wxDefaultSize, 0 );
	bSeveritySizer->Add( m_showAll, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxLEFT, 5 );


	bSeveritySizer->Add( 35, 0, 0, wxEXPAND, 5 );

	m_showErrors = new wxCheckBox( this, wxID_ANY, _("Errors"), wxDefaultPosition, wxDefaultSize, 0 );
	bSeveritySizer->Add( m_showErrors, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxLEFT, 5 );

	m_errorsBadge = new wxStaticBitmap( this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	m_errorsBadge->SetMinSize( wxSize( 20,20 ) );

	bSeveritySizer->Add( m_errorsBadge, 0, wxRIGHT|wxALIGN_CENTER_VERTICAL, 25 );

	m_showWarnings = new wxCheckBox( this, wxID_ANY, _("Warnings"), wxDefaultPosition, wxDefaultSize, 0 );
	bSeveritySizer->Add( m_showWarnings, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxLEFT, 5 );

	m_warningsBadge = new wxStaticBitmap( this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	m_warningsBadge->SetMinSize( wxSize( 20,20 ) );

	bSeveritySizer->Add( m_warningsBadge, 0, wxRIGHT|wxALIGN_CENTER_VERTICAL, 25 );

	m_showExclusions = new wxCheckBox( this, wxID_ANY, _("Exclusions"), wxDefaultPosition, wxDefaultSize, 0 );
	bSeveritySizer->Add( m_showExclusions, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxLEFT, 5 );

	m_exclusionsBadge = new wxStaticBitmap( this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSeveritySizer->Add( m_exclusionsBadge, 0, wxRIGHT|wxALIGN_CENTER_VERTICAL, 25 );


	bSeveritySizer->Add( 5, 0, 1, wxEXPAND, 5 );

	m_saveReport = new wxButton( this, wxID_ANY, _("Save..."), wxDefaultPosition, wxDefaultSize, 0 );
	bSeveritySizer->Add( m_saveReport, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxLEFT, 5 );


	bSizer9->Add( bSeveritySizer, 0, wxEXPAND|wxTOP|wxRIGHT|wxLEFT, 5 );


	m_MainSizer->Add( bSizer9, 0, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );

	m_staticline1 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	m_MainSizer->Add( m_staticline1, 0, wxEXPAND|wxTOP|wxRIGHT|wxLEFT, 10 );

	m_sizerButtons = new wxBoxSizer( wxHORIZONTAL );

	m_DeleteCurrentMarkerButton = new wxButton( this, wxID_ANY, _("Delete Marker"), wxDefaultPosition, wxDefaultSize, 0 );
	m_sizerButtons->Add( m_DeleteCurrentMarkerButton, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxLEFT, 10 );

	m_DeleteAllMarkersButton = new wxButton( this, wxID_ANY, _("Delete All Markers"), wxDefaultPosition, wxDefaultSize, 0 );
	m_sizerButtons->Add( m_DeleteAllMarkersButton, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT, 5 );

	m_sdbSizer = new wxStdDialogButtonSizer();
	m_sdbSizerOK = new wxButton( this, wxID_OK );
	m_sdbSizer->AddButton( m_sdbSizerOK );
	m_sdbSizerCancel = new wxButton( this, wxID_CANCEL );
	m_sdbSizer->AddButton( m_sdbSizerCancel );
	m_sdbSizer->Realize();

	m_sizerButtons->Add( m_sdbSizer, 1, wxEXPAND|wxALL, 5 );


	m_MainSizer->Add( m_sizerButtons, 0, wxEXPAND|wxLEFT, 5 );


	this->SetSizer( m_MainSizer );
	this->Layout();
	m_MainSizer->Fit( this );

	// Connect Events
	this->Connect( wxEVT_ACTIVATE, wxActivateEventHandler( DIALOG_DRC_BASE::OnActivateDlg ) );
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( DIALOG_DRC_BASE::OnClose ) );
	m_Notebook->Connect( wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED, wxNotebookEventHandler( DIALOG_DRC_BASE::OnChangingNotebookPage ), NULL, this );
	m_markerDataView->Connect( wxEVT_COMMAND_DATAVIEW_ITEM_ACTIVATED, wxDataViewEventHandler( DIALOG_DRC_BASE::OnDRCItemDClick ), NULL, this );
	m_markerDataView->Connect( wxEVT_COMMAND_DATAVIEW_ITEM_CONTEXT_MENU, wxDataViewEventHandler( DIALOG_DRC_BASE::OnDRCItemRClick ), NULL, this );
	m_markerDataView->Connect( wxEVT_COMMAND_DATAVIEW_SELECTION_CHANGED, wxDataViewEventHandler( DIALOG_DRC_BASE::OnDRCItemSelected ), NULL, this );
	m_unconnectedDataView->Connect( wxEVT_COMMAND_DATAVIEW_ITEM_ACTIVATED, wxDataViewEventHandler( DIALOG_DRC_BASE::OnDRCItemDClick ), NULL, this );
	m_unconnectedDataView->Connect( wxEVT_COMMAND_DATAVIEW_SELECTION_CHANGED, wxDataViewEventHandler( DIALOG_DRC_BASE::OnDRCItemSelected ), NULL, this );
	m_footprintsDataView->Connect( wxEVT_COMMAND_DATAVIEW_ITEM_ACTIVATED, wxDataViewEventHandler( DIALOG_DRC_BASE::OnDRCItemDClick ), NULL, this );
	m_footprintsDataView->Connect( wxEVT_COMMAND_DATAVIEW_SELECTION_CHANGED, wxDataViewEventHandler( DIALOG_DRC_BASE::OnDRCItemSelected ), NULL, this );
	m_showAll->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( DIALOG_DRC_BASE::OnSeverity ), NULL, this );
	m_showErrors->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( DIALOG_DRC_BASE::OnSeverity ), NULL, this );
	m_showWarnings->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( DIALOG_DRC_BASE::OnSeverity ), NULL, this );
	m_showExclusions->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( DIALOG_DRC_BASE::OnSeverity ), NULL, this );
	m_saveReport->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DIALOG_DRC_BASE::OnSaveReport ), NULL, this );
	m_DeleteCurrentMarkerButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DIALOG_DRC_BASE::OnDeleteOneClick ), NULL, this );
	m_DeleteAllMarkersButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DIALOG_DRC_BASE::OnDeleteAllClick ), NULL, this );
	m_sdbSizerCancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DIALOG_DRC_BASE::OnCancelClick ), NULL, this );
	m_sdbSizerOK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DIALOG_DRC_BASE::OnRunDRCClick ), NULL, this );
}

DIALOG_DRC_BASE::~DIALOG_DRC_BASE()
{
	// Disconnect Events
	this->Disconnect( wxEVT_ACTIVATE, wxActivateEventHandler( DIALOG_DRC_BASE::OnActivateDlg ) );
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( DIALOG_DRC_BASE::OnClose ) );
	m_Notebook->Disconnect( wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED, wxNotebookEventHandler( DIALOG_DRC_BASE::OnChangingNotebookPage ), NULL, this );
	m_markerDataView->Disconnect( wxEVT_COMMAND_DATAVIEW_ITEM_ACTIVATED, wxDataViewEventHandler( DIALOG_DRC_BASE::OnDRCItemDClick ), NULL, this );
	m_markerDataView->Disconnect( wxEVT_COMMAND_DATAVIEW_ITEM_CONTEXT_MENU, wxDataViewEventHandler( DIALOG_DRC_BASE::OnDRCItemRClick ), NULL, this );
	m_markerDataView->Disconnect( wxEVT_COMMAND_DATAVIEW_SELECTION_CHANGED, wxDataViewEventHandler( DIALOG_DRC_BASE::OnDRCItemSelected ), NULL, this );
	m_unconnectedDataView->Disconnect( wxEVT_COMMAND_DATAVIEW_ITEM_ACTIVATED, wxDataViewEventHandler( DIALOG_DRC_BASE::OnDRCItemDClick ), NULL, this );
	m_unconnectedDataView->Disconnect( wxEVT_COMMAND_DATAVIEW_SELECTION_CHANGED, wxDataViewEventHandler( DIALOG_DRC_BASE::OnDRCItemSelected ), NULL, this );
	m_footprintsDataView->Disconnect( wxEVT_COMMAND_DATAVIEW_ITEM_ACTIVATED, wxDataViewEventHandler( DIALOG_DRC_BASE::OnDRCItemDClick ), NULL, this );
	m_footprintsDataView->Disconnect( wxEVT_COMMAND_DATAVIEW_SELECTION_CHANGED, wxDataViewEventHandler( DIALOG_DRC_BASE::OnDRCItemSelected ), NULL, this );
	m_showAll->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( DIALOG_DRC_BASE::OnSeverity ), NULL, this );
	m_showErrors->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( DIALOG_DRC_BASE::OnSeverity ), NULL, this );
	m_showWarnings->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( DIALOG_DRC_BASE::OnSeverity ), NULL, this );
	m_showExclusions->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( DIALOG_DRC_BASE::OnSeverity ), NULL, this );
	m_saveReport->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DIALOG_DRC_BASE::OnSaveReport ), NULL, this );
	m_DeleteCurrentMarkerButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DIALOG_DRC_BASE::OnDeleteOneClick ), NULL, this );
	m_DeleteAllMarkersButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DIALOG_DRC_BASE::OnDeleteAllClick ), NULL, this );
	m_sdbSizerCancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DIALOG_DRC_BASE::OnCancelClick ), NULL, this );
	m_sdbSizerOK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DIALOG_DRC_BASE::OnRunDRCClick ), NULL, this );

}
