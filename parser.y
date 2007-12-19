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
%token T_VAR

%token T_PROCEDURE
%token T_FUNCTION
%token T_BEGIN
%token T_END

%token T_IF
%token T_ELSE
%token T_WHILE
%token T_FOR
%token T_TO
%token T_DO

%token T_PRINT_INT
%token T_PRINT_CHAR
%token T_PRINT_BOOL
%token T_PRINT_LINE

%token T_ASSIGNMENT
%token T_ADD
%token T_SUB
%token T_DIV
%token T_MULT
%token T_AND
%token T_OR
%token T_NOT

%token T_MAJOR
%token T_MINOR
%token T_EQUAL
%token T_NOTEQUAL
%token T_MAJOREQUAL
%token T_MINOREQUAL

%token T_LPAR
%token T_RPAR
%token T_SEMICOLON
%token T_COLON
%token T_COMMA
%token T_DOT

%token <type> TYPE_IDENTIFIER
%token <lexeme> IDENTIFIER
%token <integer> INT_LITERAL
%token <boolean> BOOL_LITERAL
%token <character> CHAR_LITERAL

/* Tipos das Regras */
%type <astnode> Program

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
%type <astnode> WhileStatement
%type <astnode> ForStatement

%type <astnode> Expression
%type <astnode> SimpleExpression
%type <astnode> ComplexExpression
%type <astnode> AddExpression
%type <astnode> MultExpression

%type <astnode> Assignment
%type <astnode> Identifier
%type <astnode> SimpleType
%type <astnode> Literal

%start Program

%%
Program:
    VarDeclList ProcFuncList MainCodeBlock
    {
        struct AstNode *ast_node;
        ast_node = ast_node_new("Program", -1, -1,
                                yylloc.last_line, NULL);
        ast_node_add_child(ast_node, $1);
        ast_node_add_child(ast_node, $2);
        ast_node_add_child(ast_node, $3);
        $$ = ast_node;

        ast_node_print(ast_node);
        //symbol_table_dump(sym_table);
        //ast_node_print_graph(ast_node);
    }
    ;

VarDeclList:
    MultiVarDecl
    {
        struct AstNode *ast_node;
        ast_node = ast_node_new("VarDeclList", -1, -1,
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
    T_VAR IdentifierList T_COLON SimpleType T_SEMICOLON
    {
        struct AstNode *ast_node;
        ast_node = ast_node_new("VarDecl", -1, -1,
                                yylloc.last_line, NULL);
        ast_node_add_child(ast_node, $2);
        ast_node_add_child(ast_node, $4);
        $$ = ast_node;
    }
    ;

IdentifierList:
    SingleIdentifier MultiIdentifier
    {
        struct AstNode *ast_node;
        ast_node = ast_node_new("IdentifierList", -1, -1,
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
        ast_node = ast_node_new("ProcFuncList", -1, -1,
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
        struct AstNode *ast_node;
        ast_node = ast_node_new("ProcDecl", -1, PROCEDURE,
                                yylloc.last_line, NULL);
        ast_node_add_child(ast_node, $2);  // Identifier
        ast_node_add_child(ast_node, $4);  // ParamList
        ast_node_add_child(ast_node, $7);  // VarDeclList
        ast_node_add_child(ast_node, $8);  // CodeBlock
        $$ = ast_node;
    }
    ;

FuncDecl:
    T_FUNCTION Identifier T_LPAR ParamList T_RPAR T_COLON SimpleType
    T_SEMICOLON VarDeclList CodeBlock T_SEMICOLON
    {
        struct AstNode *ast_node;
        ast_node = ast_node_new("FuncDecl", -1, FUNCTION,
                                yylloc.last_line, NULL);
        ast_node_add_child(ast_node, $2);  // Identifier
        ast_node_add_child(ast_node, $4);  // ParamList
        ast_node_add_child(ast_node, $7);  // SimpleType
        ast_node_add_child(ast_node, $9);  // VarDeclList
        ast_node_add_child(ast_node, $10); // CodeBlock
        $$ = ast_node;
    }
    ;

ParamList:
    /* empty */ { $$ = NULL; }
    | SingleParam MultiParam
    {
        struct AstNode *ast_node;
        ast_node = ast_node_new("ParamList", -1, -1,
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
    Identifier T_COLON SimpleType
    {
        struct AstNode *ast_node;
        ast_node = ast_node_new("SingleParam", -1, -1,
                                yylloc.last_line, NULL);
        ast_node_add_child(ast_node, $1);  // Identifier
        ast_node_add_child(ast_node, $3);  // SimpleType
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
    | Statement MultiStatement
    {
        struct AstNode *ast_node;
        ast_node = ast_node_new("StatementList", -1, -1,
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
    Assignment { $$ = $1; }
    | WhileStatement { $$ = $1; }
    | ForStatement { $$ = $1; }
    ;

Expression:
    /* empty */ { $$ = NULL; }
    | SimpleExpression ComplexExpression
    {
        struct AstNode *ast_node;
        ast_node = ast_node_new("Expression", -1, -1,
                                yylloc.last_line, NULL);
        ast_node_add_child(ast_node, $1);
        ast_node_add_child(ast_node, $2);
        $$ = ast_node;
    }
    ;

SimpleExpression:
    AddExpression { $$ = $1; }
    | MultExpression { $$ = $1; }
    | Literal { $$ = $1; }
    | Identifier { $$ = $1; }
    ;

AddExpression:
    T_ADD SimpleExpression { $$ = $2; }
    ;

MultExpression:
    T_MULT SimpleExpression { $$ = $2; }
    ;

ComplexExpression:
    /* empty */ { $$ = NULL; }
    | RelOp SimpleExpression { $$ = NULL; }
    ;

RelOp:
    T_MINOR
    | T_MINOREQUAL
    | T_MAJOR
    | T_MAJOREQUAL
    | T_EQUAL
    | T_NOTEQUAL
    ;

Assignment:
    Identifier T_ASSIGNMENT Expression T_SEMICOLON
    {
        struct AstNode *ast_node;
        ast_node = ast_node_new("Assignment", -1, -1,
                                yylloc.last_line, NULL);
        ast_node_add_child(ast_node, $1);
        ast_node_add_child(ast_node, $3);
        //ast_node_print($3);
        $$ = ast_node;
    }
    ;

WhileStatement:
    T_WHILE T_LPAR Expression T_RPAR T_DO CodeBlock
    {
        struct AstNode *ast_node;
        ast_node = ast_node_new("WhileStatement", -1, -1,
                                yylloc.last_line, NULL);
        ast_node_add_child(ast_node, $3);
        ast_node_add_child(ast_node, $6);
        $$ = ast_node;
    }
    ;

ForStatement:
    T_FOR Identifier T_ASSIGNMENT Expression T_TO Expression T_DO CodeBlock
    {
        struct AstNode *ast_node;
        ast_node = ast_node_new("ForStatement", -1, -1,
                                yylloc.last_line, NULL);
        ast_node_add_child(ast_node, $2);
        ast_node_add_child(ast_node, $4);
        ast_node_add_child(ast_node, $6);
        ast_node_add_child(ast_node, $8);
        $$ = ast_node;
    }
    ;

SimpleType:
    TYPE_IDENTIFIER
    {
        struct AstNode *ast_node;
        ast_node = ast_node_new("SimpleType", -1, $1,
                                yylloc.last_line, NULL);
        $$ = ast_node;
    }
    ;

Identifier:
    IDENTIFIER
    {
        Symbol *symbol;
        struct AstNode *ast_node;

        //sym_table = symbol_insert(sym_table, $1);
        symbol_add(sym_table, $1);
        symbol = sym_table;

        ast_node = ast_node_new("Identifier", -1, -1,
                                yylloc.last_line, NULL);
        ast_node->symbol = symbol;
        $$ = ast_node;
    }
    ;

Literal:
    INT_LITERAL
    {
        struct AstNode *ast_node;
        ast_node = ast_node_new("Literal", INT_LITERAL, INTEGER,
                                yylloc.last_line, NULL);
        value_set_from_int(&ast_node->value, $1);
        $$ = ast_node;
    }
    | BOOL_LITERAL
    {
        struct AstNode *ast_node;
        ast_node = ast_node_new("Literal", BOOL_LITERAL, BOOLEAN,
                                yylloc.last_line, NULL);
        value_set_from_int(&ast_node->value, $1);
        $$ = ast_node;
    }
    | CHAR_LITERAL
    {
        struct AstNode *ast_node;
        ast_node = ast_node_new("Literal", CHAR_LITERAL, CHAR,
                                yylloc.last_line, NULL);
        value_set_from_int(&ast_node->value, $1);
        $$ = ast_node;
    }
    ;

%%

static void
yyerror (/*YYLTYPE *locp,*/ const char *msg)
{
    fprintf(stderr,"error: %s\n", msg);
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

    symbol_table_init();

    return yyparse();
}

