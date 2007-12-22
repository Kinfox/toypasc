%{
#include <stdio.h>
#include <string.h>
#include "base.h"
#include "parser.h"
#include "ast.h"
#include "symbol_table.h"

/*extern char *yytext;*/
extern FILE *yyin;

static void yyerror (/*YYLTYPE *locp, */const char *msg);
/*int yylex (YYSTYPE *yylval_param, YYLTYPE *yylloc_param);*/
%}

%defines
%locations
%pure-parser
%error-verbose
/*%parse-param {ValaParser *parser}
%lex-param {ValaParser *parser}*/

%union {
    char* lexeme;
    int integer;
    int boolean;
    char character;
    int type;
    struct AstNode *astnode;
}

/* Tokens */
%left <lexeme> T_OR
%left <lexeme> T_AND
%left <lexeme> T_EQUAL T_NOTEQUAL
%left <lexeme> T_LESSER T_GREATER T_LESSEREQUAL T_GREATEREQUAL
%left <lexeme> T_PLUS T_MINUS
%left <lexeme> T_STAR T_SLASH
%left <lexeme> T_NOT

%token T_PROGRAM
%token T_VAR
%token T_PROCEDURE
%token T_FUNCTION
%token T_BEGIN
%token T_END

%token T_IF
%token T_THEN
%token T_ELSE
%token T_WHILE
%token T_FOR
%token T_TO
%token T_DO

%token T_ASSIGNMENT

%token T_LPAR
%token T_RPAR
%token T_SEMICOLON
%token T_COLON
%token T_COMMA
%token T_DOT

%token T_PRINT_INT
%token T_PRINT_CHAR
%token T_PRINT_BOOL
%token T_PRINT_LINE

%token <type> TYPE_IDENTIFIER
%token <lexeme> IDENTIFIER
%token <integer> INT_LITERAL
%token <boolean> BOOL_LITERAL
%token <character> CHAR_LITERAL

/* Tipos das Regras */
%type <astnode> Program
%type <astnode> ProgramDecl
%type <astnode> VarDeclList
%type <astnode> MultiVarDecl
%type <astnode> VarDecl
%type <astnode> IdentifierList
%type <astnode> MultiIdentifier
%type <astnode> SingleIdentifier

%type <astnode> ProcFuncList
%type <astnode> MultiProcFuncDecl
%type <astnode> ProcFuncDecl
%type <astnode> ProcDecl
%type <astnode> FuncDecl
%type <astnode> ParamList
%type <astnode> SingleParam
%type <astnode> MultiParam

%type <astnode> MainCodeBlock
%type <astnode> CodeBlock
%type <astnode> StatementList
%type <astnode> MultiStatement
%type <astnode> Statement
%type <astnode> IfStatement
%type <astnode> ElseStatement
%type <astnode> WhileStatement
%type <astnode> ForStatement
%type <astnode> PrintStatement
%type <astnode> PrintCharStatement
%type <astnode> PrintIntStatement
%type <astnode> PrintBoolStatement
%type <astnode> PrintLineStatement

%type <astnode> Expression
%type <astnode> SimpleExpression
%type <astnode> NotFactor
%type <astnode> Factor
%type <astnode> Term

%type <astnode> Call
%type <astnode> CallParamList
%type <astnode> MultiCallParam

%type <astnode> Assignment
%type <astnode> Identifier
%type <astnode> Literal

%type <astnode> AddOp
%type <astnode> MulOp
%type <astnode> RelOp
%type <astnode> NotOp

%start Program

%%
Program:
    ProgramDecl VarDeclList ProcFuncList MainCodeBlock
    {
        struct AstNode *ast_node;
        struct AstNode *temp;
        ast_node = ast_node_new("Program", PROGRAM, NONE_TYPE,
                                yylloc.last_line, NULL);
        ast_node_add_child(ast_node, $1);
        ast_node_add_child(ast_node, $2);
        ast_node_add_child(ast_node, $3);
        ast_node_add_child(ast_node, $4);
        $$ = ast_node;

        global_symbol_table = symbol_new(NULL);
        ast_node->symbol = global_symbol_table;
        for (temp = ast_node->children; temp != NULL; temp = temp->sibling)
            ast_node_fill_symbol_table(temp, global_symbol_table);

        //ast_node_print(ast_node);
        ast_node_print_graph(ast_node);
        //symbol_table_dump(ast_node->symbol);
    }
    ;

ProgramDecl:
    T_PROGRAM Identifier T_SEMICOLON
    {
        struct AstNode *ast_node;
        ast_node = ast_node_new("ProgramDecl", PROG_DECL, NONE_TYPE,
                                yylloc.last_line, NULL);
        ast_node_add_child(ast_node, $2);
        $$ = ast_node;
    }
    ;

VarDeclList:
    /* empty */ { $$ = NULL; }
    | MultiVarDecl
    {
        struct AstNode *ast_node;
        ast_node = ast_node_new("VarDeclList", NONE_KIND, NONE_TYPE,
                                yylloc.last_line, NULL);
        ast_node_add_child(ast_node, $1);
        $$ = ast_node;
    }
    ;

MultiVarDecl:
    /* empty */ { $$ = NULL; }
    | VarDecl MultiVarDecl
    {
        ast_node_add_sibling($1, $2);
        $$ = $1;
    }
    ;

VarDecl:
    T_VAR IdentifierList T_COLON TYPE_IDENTIFIER T_SEMICOLON
    {
        struct AstNode *ast_node;
        ast_node = ast_node_new("VarDecl", NONE_TYPE, $4,
                                yylloc.last_line, NULL);
        ast_node_add_child(ast_node, $2);
        ast_node_set_type($2, $4);
        $$ = ast_node;
    }
    ;

IdentifierList:
    SingleIdentifier MultiIdentifier
    {
        struct AstNode *ast_node;
        ast_node = ast_node_new("IdentifierList", IDENT_LIST, NONE_TYPE,
                                yylloc.last_line, NULL);
        ast_node_add_sibling($1, $2);
        ast_node_add_child(ast_node, $1);
        $$ = ast_node;
    }
    ;

MultiIdentifier:
    /* empty */ { $$ = NULL; }
    | T_COMMA SingleIdentifier MultiIdentifier
    {
        ast_node_add_sibling($2, $3);
        $$ = $2;
    }
    ;

SingleIdentifier:
    Identifier { $$ = $1; }
    ;


ProcFuncList:
    /* empty */ { $$ = NULL; }
    | ProcFuncDecl MultiProcFuncDecl
    {
        struct AstNode *ast_node;
        ast_node = ast_node_new("ProcFuncList", NONE_KIND, NONE_TYPE,
                                yylloc.last_line, NULL);
        ast_node_add_sibling($1, $2);
        ast_node_add_child(ast_node, $1);
        $$ = ast_node;
    }
    ;

MultiProcFuncDecl:
    /* empty */ { $$ = NULL; }
    | ProcFuncDecl MultiProcFuncDecl
    {
        ast_node_add_sibling($1, $2);
        $$ = $1;
    }
    ;

ProcFuncDecl:
    ProcDecl { $$ = $1; }
    | FuncDecl { $$ = $1; }
    ;

ProcDecl:
    T_PROCEDURE Identifier T_LPAR ParamList T_RPAR T_SEMICOLON VarDeclList
    CodeBlock T_SEMICOLON
    {
        Symbol *symtab;
        struct AstNode *ast_node;
        struct AstNode *temp;

        ast_node = ast_node_new("ProcDecl", PROCEDURE, NONE_TYPE,
                                yylloc.last_line, NULL);
        ast_node_add_child(ast_node, $2);  // Identifier
        ast_node_add_child(ast_node, $4);  // ParamList
        ast_node_add_child(ast_node, $7);  // VarDeclList
        ast_node_add_child(ast_node, $8);  // CodeBlock

        symtab = symbol_new(NULL);
        ast_node->symbol = symtab;
        for (temp = ast_node->children; temp != NULL; temp = temp->sibling)
            ast_node_fill_symbol_table(temp, symtab);

        $$ = ast_node;
    }
    ;

FuncDecl:
    T_FUNCTION Identifier T_LPAR ParamList T_RPAR T_COLON TYPE_IDENTIFIER
    T_SEMICOLON VarDeclList CodeBlock T_SEMICOLON
    {
        Symbol *symtab;
        struct AstNode *ast_node;
        struct AstNode *temp;

        ast_node = ast_node_new("FuncDecl", FUNCTION, $7,
                                yylloc.last_line, NULL);
        ast_node_add_child(ast_node, $2);  // Identifier
        ast_node_add_child(ast_node, $4);  // ParamList
        ast_node_add_child(ast_node, $9);  // VarDeclList
        ast_node_add_child(ast_node, $10); // CodeBlock

        ast_node_set_type($2, $7);

        symtab = symbol_new(NULL);
        ast_node->symbol = symtab;
        for (temp = ast_node->children; temp != NULL; temp = temp->sibling)
            ast_node_fill_symbol_table(temp, symtab);

        $$ = ast_node;
    }
    ;

ParamList:
    /* empty */ { $$ = NULL; }
    | SingleParam MultiParam
    {
        struct AstNode *ast_node;
        ast_node = ast_node_new("ParamList", NONE_KIND, NONE_TYPE,
                                yylloc.last_line, NULL);
        ast_node_add_sibling($1, $2);
        ast_node_add_child(ast_node, $1);
        $$ = ast_node;
    }
    ;

MultiParam:
    /* empty */ { $$ = NULL; }
    | T_COMMA SingleParam MultiParam
    {
        ast_node_add_sibling($2, $3);
        $$ = $2;
    }
    ;

SingleParam:
    Identifier T_COLON TYPE_IDENTIFIER
    {
        struct AstNode *ast_node;
        ast_node = ast_node_new("SingleParam", NONE_KIND, NONE_TYPE,
                                yylloc.last_line, NULL);
        ast_node_add_child(ast_node, $1);  // Identifier
        ast_node_set_type(ast_node, $3);
        $$ = ast_node;
    }
    ;

MainCodeBlock:
    /* empty */ { $$ = NULL; }
    | CodeBlock T_DOT { $$ = $1; }
    ;

CodeBlock:
    T_BEGIN StatementList T_END { $$ = $2; }
    ;

StatementList:
    Statement MultiStatement
    {
        struct AstNode *ast_node;
        ast_node = ast_node_new("StatementList", NONE_KIND, NONE_TYPE,
                                yylloc.last_line, NULL);
        ast_node_add_sibling($1, $2);
        ast_node_add_child(ast_node, $1);
        $$ = ast_node;
    }
    ;

MultiStatement:
    /* empty */ { $$ = NULL; }
    | Statement MultiStatement
    {
        ast_node_add_sibling($1, $2);
        $$ = $1;
    }
    ;

Statement:
    Assignment T_SEMICOLON { $$ = $1; }
    | IfStatement { $$ = $1; }
    | WhileStatement { $$ = $1; }
    | ForStatement { $$ = $1; }
    | Call { $$ = $1; }
    | PrintStatement { $$ = $1; }
    ;

PrintStatement:
    PrintIntStatement { $$ = $1; }
    | PrintCharStatement { $$ = $1; }
    | PrintBoolStatement { $$ = $1; }
    | PrintLineStatement { $$ = $1; }
    ;

PrintIntStatement:
    T_PRINT_INT T_LPAR Expression T_RPAR
    {
        struct AstNode *ast_node;
        ast_node = ast_node_new("PrintIntStatement", NONE_KIND, NONE_TYPE,
                                yylloc.last_line, NULL);
        ast_node_add_child(ast_node, $3);
        $$ = ast_node;
    }
    ;

PrintCharStatement:
    T_PRINT_CHAR T_LPAR Expression T_RPAR
    {
        struct AstNode *ast_node;
        ast_node = ast_node_new("PrintCharStatement", NONE_KIND, NONE_TYPE,
                                yylloc.last_line, NULL);
        ast_node_add_child(ast_node, $3);
        $$ = ast_node;
    }
    ;

PrintBoolStatement:
    T_PRINT_BOOL T_LPAR Expression T_RPAR
    {
        struct AstNode *ast_node;
        ast_node = ast_node_new("PrintBoolStatement", NONE_KIND, NONE_TYPE,
                                yylloc.last_line, NULL);
        ast_node_add_child(ast_node, $3);
        $$ = ast_node;
    }
    ;

PrintLineStatement:
    T_PRINT_LINE T_LPAR Expression T_RPAR
    {
        struct AstNode *ast_node;
        ast_node = ast_node_new("PrintLineStatement", NONE_KIND, NONE_TYPE,
                                yylloc.last_line, NULL);
        ast_node_add_child(ast_node, $3);
        $$ = ast_node;
    }
    ;

Assignment:
    Identifier T_ASSIGNMENT Expression
    {
        struct AstNode *ast_node;
        ast_node = ast_node_new("Assignment", NONE_KIND, NONE_TYPE,
                                yylloc.last_line, NULL);
        ast_node_add_child(ast_node, $1);
        ast_node_add_child(ast_node, $3);
        $$ = ast_node;
    }
    ;

IfStatement:
    T_IF Expression T_THEN CodeBlock ElseStatement
    {
        struct AstNode *ast_node;
        ast_node = ast_node_new("IfStatement", NONE_KIND, NONE_TYPE,
                                yylloc.last_line, NULL);
        ast_node_add_child(ast_node, $2);
        ast_node_add_child(ast_node, $4);
        ast_node_add_child(ast_node, $5);
        $$ = ast_node;
    }
    ;

ElseStatement:
    /* empty */ { $$ = NULL; }
    | T_ELSE CodeBlock { $$ = $2; }
    ;

WhileStatement:
    T_WHILE T_LPAR Expression T_RPAR T_DO CodeBlock
    {
        struct AstNode *ast_node;
        ast_node = ast_node_new("WhileStatement", NONE_KIND, NONE_TYPE,
                                yylloc.last_line, NULL);
        ast_node_add_child(ast_node, $3);
        ast_node_add_child(ast_node, $6);
        $$ = ast_node;
    }
    ;

ForStatement:
    T_FOR Assignment T_TO Expression T_DO CodeBlock
    {
        struct AstNode *ast_node;
        ast_node = ast_node_new("ForStatement", NONE_KIND, NONE_TYPE,
                                yylloc.last_line, NULL);
        ast_node_add_child(ast_node, $2);
        ast_node_add_child(ast_node, $4);
        ast_node_add_child(ast_node, $6);
        $$ = ast_node;
    }
    ;

Expression:
    SimpleExpression { $$ = $1; }
    | SimpleExpression RelOp SimpleExpression
    {
        struct AstNode *ast_node;
        ast_node = ast_node_new("RelExpression", NONE_KIND, NONE_TYPE,
                                yylloc.last_line, NULL);
        ast_node_add_child(ast_node, $1);
        ast_node_add_child(ast_node, $2);
        ast_node_add_child(ast_node, $3);
        $$ = ast_node;
    }
    ;

SimpleExpression:
    Term { $$ = $1; }
    | SimpleExpression AddOp Term
    {
        struct AstNode *ast_node;
        ast_node = ast_node_new("AddExpression", NONE_KIND, NONE_TYPE,
                                yylloc.last_line, NULL);
        ast_node_add_child(ast_node, $1);
        ast_node_add_child(ast_node, $2);
        ast_node_add_child(ast_node, $3);
        $$ = ast_node;
    }
    ;

Term:
    NotFactor { $$ = $1; }
    | Term MulOp NotFactor
    {
        struct AstNode *ast_node;
        ast_node = ast_node_new("MulExpression", NONE_KIND, NONE_TYPE,
                                yylloc.last_line, NULL);
        ast_node_add_child(ast_node, $1);
        ast_node_add_child(ast_node, $2);
        ast_node_add_child(ast_node, $3);
        $$ = ast_node;
    }
    ;

NotFactor:
    Factor { $$ = $1; }
    | NotOp Factor
    {
        struct AstNode *ast_node;
        struct AstNode *op_ast_node;
        ast_node = ast_node_new("NotFactor", NONE_KIND, NONE_TYPE,
                                yylloc.last_line, NULL);
        ast_node_add_child(ast_node, $1);
        ast_node_add_child(ast_node, $2);
        $$ = ast_node;
    }
    ;

Factor:
    Identifier { $$ = $1; }
    | Literal { $$ = $1; }
    | Call { $$ = $1; }
    | T_LPAR Expression T_RPAR { $$ = $2; }
    ;

Call:
    Identifier T_LPAR CallParamList T_RPAR
    {
        struct AstNode *ast_node;
        ast_node = ast_node_new("Call", NONE_KIND, NONE_TYPE,
                                yylloc.last_line, NULL);
        ast_node_add_child(ast_node, $1);
        ast_node_add_child(ast_node, $3);
        $$ = ast_node;
    }
    ;

CallParamList:
    /* empty */ { $$ = NULL; }
    | Expression MultiCallParam
    {
        struct AstNode *ast_node;
        ast_node = ast_node_new("CallParamList", NONE_KIND, NONE_TYPE,
                                yylloc.last_line, NULL);
        ast_node_add_sibling($1, $2);
        ast_node_add_child(ast_node, $1);
        $$ = ast_node;
    }
    ;

MultiCallParam:
    /* empty */ { $$ = NULL; }
    | T_COMMA Expression MultiCallParam
    {
        ast_node_add_sibling($2, $3);
        $$ = $2;
    }
    ;

AddOp:
    T_PLUS
    {
        struct AstNode *ast_node;
        ast_node = ast_node_new($1, T_PLUS, NONE_TYPE,
                                yylloc.last_line, NULL);
        $$ = ast_node;
    }
    | T_MINUS
    {
        struct AstNode *ast_node;
        ast_node = ast_node_new($1, T_MINUS, NONE_TYPE,
                                yylloc.last_line, NULL);
        $$ = ast_node;
    }
    | T_OR
    {
        struct AstNode *ast_node;
        ast_node = ast_node_new($1, T_OR, NONE_TYPE,
                                yylloc.last_line, NULL);
        $$ = ast_node;
    }
    ;

MulOp:
    T_STAR
    {
        struct AstNode *ast_node;
        ast_node = ast_node_new($1, T_STAR, NONE_TYPE,
                                yylloc.last_line, NULL);
        $$ = ast_node;
    }
    | T_SLASH
    {
        struct AstNode *ast_node;
        ast_node = ast_node_new($1, T_SLASH, NONE_TYPE,
                                yylloc.last_line, NULL);
        $$ = ast_node;
    }
    | T_AND
    {
        struct AstNode *ast_node;
        ast_node = ast_node_new($1, T_AND, NONE_TYPE,
                                yylloc.last_line, NULL);
        $$ = ast_node;
    }
    ;

RelOp:
    T_LESSER
    {
        struct AstNode *ast_node;
        ast_node = ast_node_new($1, T_LESSER, NONE_TYPE,
                                yylloc.last_line, NULL);
        $$ = ast_node;
    }
    | T_LESSEREQUAL
    {
        struct AstNode *ast_node;
        ast_node = ast_node_new($1, T_LESSEREQUAL, NONE_TYPE,
                                yylloc.last_line, NULL);
        $$ = ast_node;
    }
    | T_GREATER
    {
        struct AstNode *ast_node;
        ast_node = ast_node_new($1, T_GREATER, NONE_TYPE,
                                yylloc.last_line, NULL);
        $$ = ast_node;
    }
    | T_GREATEREQUAL
    {
        struct AstNode *ast_node;
        ast_node = ast_node_new($1, T_GREATEREQUAL, NONE_TYPE,
                                yylloc.last_line, NULL);
        $$ = ast_node;
    }
    | T_EQUAL
    {
        struct AstNode *ast_node;
        ast_node = ast_node_new($1, T_EQUAL, NONE_TYPE,
                                yylloc.last_line, NULL);
        $$ = ast_node;
    }
    | T_NOTEQUAL
    {
        struct AstNode *ast_node;
        ast_node = ast_node_new($1, T_NOTEQUAL, NONE_TYPE,
                                yylloc.last_line, NULL);
        $$ = ast_node;
    }
    ;

NotOp:
    T_NOT
    {
        struct AstNode *ast_node;
        ast_node = ast_node_new($1, T_NOT, NONE_TYPE,
                                yylloc.last_line, NULL);
        $$ = ast_node;
    }
    ;

Identifier:
    IDENTIFIER
    {
        Symbol *symbol;
        struct AstNode *ast_node;

        ast_node = ast_node_new("Identifier", IDENTIFIER, NONE_TYPE,
                                yylloc.last_line, NULL);
        ast_node->symbol = symbol_new($1);
        $$ = ast_node;
    }
    ;

Literal:
    INT_LITERAL
    {
        struct AstNode *ast_node;
        ast_node = ast_node_new("IntLiteral", INT_LITERAL, INTEGER,
                                yylloc.last_line, NULL);
        value_set_from_int(&ast_node->value, $1);
        $$ = ast_node;
    }
    | BOOL_LITERAL
    {
        struct AstNode *ast_node;
        ast_node = ast_node_new("BoolLiteral", BOOL_LITERAL, BOOLEAN,
                                yylloc.last_line, NULL);
        value_set_from_bool(&ast_node->value, $1);
        $$ = ast_node;
    }
    | CHAR_LITERAL
    {
        struct AstNode *ast_node;
        ast_node = ast_node_new("CharLiteral", CHAR_LITERAL, CHAR,
                                yylloc.last_line, NULL);
        value_set_from_char(&ast_node->value, $1);
        $$ = ast_node;
    }
    ;

%%

static void
yyerror (/*YYLTYPE *locp,*/ const char *msg)
{
    fprintf(stderr, "[Error] line %d: %s\n", yyget_lineno(), msg);
}

int
main(int argc, char **argv)
{
    if (argc > 1)
        yyin = fopen(argv[1], "r");
    else
        yyin = stdin;

    /*yylloc.first_line = yylloc.last_line = 1;
    yylloc.first_column = yylloc.last_column = 0;*/

    return yyparse();
}

