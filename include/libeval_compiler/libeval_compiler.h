/*
    This file is part of libeval, a simple math expression evaluator

    Copyright (C) 2007 Michael Geselbracht, mgeselbracht3@gmail.com
    Copyright (C) 2019-2020 KiCad Developers, see AUTHORS.txt for contributors.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef __LIBEVAL_COMPILER_H
#define __LIBEVAL_COMPILER_H

#include <cstddef>
#include <functional>
#include <map>
#include <string>

#include <base_units.h>

#define TR_OP_BINARY_MASK 0x200
#define TR_OP_UNARY_MASK 0x100

#define TR_OP_MUL 0x201
#define TR_OP_DIV 0x202
#define TR_OP_ADD 0x203
#define TR_OP_SUB 0x204
#define TR_OP_LESS 0x25
#define TR_OP_GREATER 0x206
#define TR_OP_LESS_EQUAL 0x207
#define TR_OP_GREATER_EQUAL 0x208
#define TR_OP_EQUAL 0x209
#define TR_OP_NOT_EQUAL 0x20a
#define TR_OP_BOOL_AND 0x20b
#define TR_OP_BOOL_OR  0x20c
#define TR_OP_BOOL_NOT 0x100
#define TR_OP_FUNC_CALL 24
#define TR_OP_METHOD_CALL 25
#define TR_UOP_PUSH_VAR 1
#define TR_UOP_PUSH_VALUE 2

// This namespace is used for the lemon parser
namespace LIBEVAL
{

class COMPILER;

struct ERROR_STATUS
{
    bool pendingError = false;

    enum STAGE
    {
        CST_PARSE = 0,
        CST_CODEGEN,
        CST_RUNTIME
    };

    STAGE    stage;
    wxString message;             // Note: use wxString for GUI-related strings
    int      srcPos;
};


enum VAR_TYPE_T
{
    VT_STRING = 1,
    VT_NUMERIC,
    VT_UNDEFINED
};

enum TOKEN_TYPE_T
{
    TR_NUMBER    = 1,
    TR_IDENTIFIER = 2,
    TR_ASSIGN     = 3,
    TR_STRUCT_REF = 4,
    TR_STRING     = 5,
    TR_UNIT = 6
};

#define LIBEVAL_MAX_LITERAL_LENGTH 1024

class UOP;

struct TREE_NODE
{
    struct value_s
    {
        char str[LIBEVAL_MAX_LITERAL_LENGTH];
        int  type;
    } value;

    int        op;
    TREE_NODE* leaf[2];
    UOP*       uop;
    bool       valid;
    bool       isTerminal;
    int        srcPos;
};


static inline TREE_NODE* copyNode( TREE_NODE& t )
{
    auto t2          = new TREE_NODE();
    t2->valid        = t.valid;
    snprintf( t2->value.str, LIBEVAL_MAX_LITERAL_LENGTH, "%s", t.value.str );
    t2->op           = t.op;
    t2->value.type   = t.value.type;
    t2->leaf[0]      = t.leaf[0];
    t2->leaf[1]      = t.leaf[1];
    t2->isTerminal   = false;
    t2->srcPos = t.srcPos;
    t2->uop          = nullptr;
    return t2;
}


static inline TREE_NODE* newNode( int op, int type, const std::string& value )
{
    auto t2          = new TREE_NODE();
    t2->valid        = true;
    snprintf( t2->value.str, LIBEVAL_MAX_LITERAL_LENGTH, "%s", value.c_str() );
    t2->op           = op;
    t2->value.type   = type;
    t2->leaf[0]      = nullptr;
    t2->leaf[1]      = nullptr;
    t2->isTerminal   = false;
    t2->srcPos = -1;
    t2->uop          = nullptr;
    return t2;
}


class UNIT_RESOLVER
{
public:
    UNIT_RESOLVER()
    {
    }

    virtual ~UNIT_RESOLVER()
    {
    }

    virtual const std::vector<std::string>& GetSupportedUnits() const
    {
        static const std::vector<std::string> nullUnits;

        return nullUnits;
    }

    virtual double Convert( const std::string& aString, int unitType ) const
    {
        return 0.0;
    };
};


class VALUE
{
public:
    VALUE():
        m_type(VT_UNDEFINED),
        m_valueDbl( 0 )
    {};

    VALUE( std::string aStr ) :
        m_type( VT_STRING ),
        m_valueDbl( 0 ),
        m_valueStr( std::move( aStr ) )
    {};

    VALUE( const double aVal ) :
        m_type( VT_NUMERIC ),
        m_valueDbl( aVal )
    {};

    double AsDouble() const
    {
        return m_valueDbl;
    }

    const std::string& AsString() const
    {
        return m_valueStr;
    }

    bool operator==( const VALUE& b ) const
    {
        if( m_type == VT_NUMERIC && b.m_type == VT_NUMERIC )
            return m_valueDbl == b.m_valueDbl;
        else if( m_type == VT_STRING && b.m_type == VT_STRING )
            return m_valueStr == b.m_valueStr;

        return false;
    }

    VAR_TYPE_T GetType() const { return m_type; };

    void Set( double aValue )
    {
        m_type = VT_NUMERIC;
        m_valueDbl = aValue;
    }

    void Set( const std::string& aValue )
    {
        m_type = VT_STRING;
        m_valueStr = aValue;
    }

    void Set( const VALUE &val )
    {
        m_type = val.m_type;
        m_valueDbl = val.m_valueDbl;

        if( m_type == VT_STRING )
            m_valueStr = val.m_valueStr;
    }

    bool EqualTo( const VALUE* v2 ) const
    {
        return operator==( *v2 );
    }

private:
    VAR_TYPE_T  m_type;
    double      m_valueDbl;
    std::string m_valueStr;
};


class UCODE;
class CONTEXT;


class VAR_REF
{
public:
    virtual VAR_TYPE_T GetType() = 0;
    virtual VALUE GetValue( CONTEXT* aCtx, UCODE* aUcode ) = 0;
};


class CONTEXT
{
public:
    const int c_memSize = 128;

    CONTEXT()
    {
        m_sp = 0;

        for( int i = 0; i < c_memSize; i++ )
            m_heap.emplace_back( VALUE() );
    }

    VALUE* AllocValue()
    {
        assert( m_memPos < c_memSize );
        auto rv = &m_heap[ m_memPos++ ];
        return rv;
    }

    void   Push( VALUE* v )
    {
        m_stack[m_sp++] = v;
    }

    VALUE* Pop()
    {
        m_sp--;
        return m_stack[m_sp];
    }

    int SP() const
    {
        return m_sp;
    }

    ERROR_STATUS GetErrorStatus() const { return m_errorStatus; }
    void ReportError( const wxString& aErrorMsg );

private:
    std::vector<VALUE> m_heap;
    VALUE*             m_stack[128];
    int                m_sp = 0;
    int                m_memPos = 0;
    ERROR_STATUS       m_errorStatus;
};


class UCODE
{
public:
    typedef std::function<void( UCODE*, CONTEXT*, void* )> FUNC_PTR;

    void AddOp( UOP* uop )
    {
        m_ucode.push_back(uop);
    }

    VALUE* Run();
    std::string Dump() const;

    virtual VAR_REF* createVarRef( COMPILER* aCompiler, const std::string& var,
                                   const std::string& field )
    {
        return nullptr;
    };

    virtual FUNC_PTR createFuncCall( COMPILER* aCompiler, const std::string& name )
    {
        return nullptr;
    };

private:
    std::vector<UOP*> m_ucode;
};


class UOP
{
public:
    UOP( int op, void* arg ) :
        m_op( op ),
        m_arg( arg )
    {};

    UOP( int op, UCODE::FUNC_PTR func, void *arg ) :
        m_op( op ),
        m_arg(arg),
        m_func( std::move( func ) )
    {};

    void Exec( CONTEXT* ctx, UCODE *ucode );

    std::string Format() const;

private:
    int             m_op;
    void*           m_arg;
    UCODE::FUNC_PTR m_func;
};

class TOKENIZER
{
public:
    void Restart( const std::string& aStr )
    {
        m_str = aStr;
        m_pos = 0;
    }

    void Clear()
    {
        m_str = "";
        m_pos = 0;
    }

    int GetChar() const
    {
        if( m_pos >= m_str.length() )
            return 0;

        return m_str[m_pos];
    }

    bool Done() const
    {
        return m_pos >= m_str.length();
    }

    void NextChar( int aAdvance = 1 )
    {
        m_pos += aAdvance;
    }

    size_t GetPos() const
    {
        return m_pos;
    }

    std::string GetChars( std::function<bool( int )> cond ) const;

    bool MatchAhead( const std::string& match, std::function<bool( int )> stopCond ) const;

private:
    std::string m_str;
    size_t      m_pos;
};


class COMPILER
{
public:
    COMPILER();
    virtual ~COMPILER();

    /*
     * clear() should be invoked by the client if a new input string is to be processed. It
     * will reset the parser. User defined variables are retained.
     */
    void Clear();

    /* Used by the lemon parser */
    void parseError( const char* s );
    void parseOk();

    int GetSourcePos() const { return m_sourcePos; }

    /* Check if previous invokation of process() was successful */
    inline bool IsValid() const
    {
        return !m_errorStatus.pendingError;
    }

    void setRoot( LIBEVAL::TREE_NODE root );

    bool Compile( const std::string& aString, UCODE* aCode );
    void ReportError( const wxString& aErrorMsg );
    ERROR_STATUS GetErrorStatus() const { return m_errorStatus; }

protected:
    enum LEXER_STATE
    {
        LS_DEFAULT = 0,
        LS_STRING  = 1,
    };

    LEXER_STATE m_lexerState;

    bool generateUCode( UCODE* aCode );


    /* Token type used by the tokenizer */
    struct T_TOKEN
    {
        int       token;
        TREE_NODE value;
    };

    /* Begin processing of a new input string */
    void newString( const std::string& aString );

    /* Tokenizer: Next token/value taken from input string. */
    T_TOKEN getToken();
    bool  lexDefault( T_TOKEN& aToken );
    bool  lexString( T_TOKEN& aToken );

    int resolveUnits();

    UOP* makeUop( int op, double value )
    {
        auto uop = new UOP( op, new VALUE( value ) );
        return uop;
    }

    UOP* makeUop( int op, std::string value )
    {
        UOP* uop = new UOP( op, new VALUE( std::move( value ) ) );
        return uop;
    }

    UOP* makeUop( int op, VAR_REF* aRef = nullptr )
    {
        UOP* uop = new UOP( op, aRef );
        return uop;
    }

    UOP* makeUop( int op, UCODE::FUNC_PTR aFunc, void *arg = nullptr )
    {
        UOP* uop = new UOP( op, std::move( aFunc ), arg );
        return uop;
    }

    /* Token state for input string. */
    
    void*        m_parser; // the current lemon parser state machine
    TOKENIZER    m_tokenizer;
    char         m_localeDecimalSeparator;

    std::unique_ptr<UNIT_RESOLVER> m_unitResolver;

    int          m_sourcePos;
    ERROR_STATUS m_errorStatus;
    bool         m_parseFinished;

    TREE_NODE*   m_tree;
};


} // namespace LIBEVAL

#endif /* LIBEVAL_COMPILER_H_ */
