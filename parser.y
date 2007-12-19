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

%type <astnode> Program
%type <astnode> VarDeclList
%type <astnode> VarDecl

%type <astnode> ProcFuncList
%type <astnode> ProcDecl
%type <astnode> FuncDecl

%type <astnode> ParamList
%type <astnode> SingleParam
%type <astnode> MultiParam

%type <astnode> MainCodeBlock
%type <astnode> CodeBlock

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
        ast_node->children[0] = $1;
        ast_node->children[1] = $2;
        ast_node->children[2] = $3;
        $$ = ast_node;

        ast_node_print(ast_node);
        //symbol_table_dump(sym_table);
        //ast_node_print_graph(ast_node);
    }
    ;

VarDeclList:
    /* empty */ { $$ = NULL; }
    | VarDeclList VarDecl
    {
        ((struct AstNode *) $2)->next = (struct AstNode *) $1;
        $$ = $2;
    }
    ;

VarDecl:
    T_VAR Identifier T_COLON SimpleType T_SEMICOLON
    {
        struct AstNode *ast_node;
        ast_node = ast_node_new("VarDecl", -1, -1,
                                yylloc.last_line, NULL);
        ast_node->children[0] = $2;
        ast_node->children[1] = $4;
        $$ = ast_node;
    }
    ;

ProcFuncList:
    /* empty */ { $$ = NULL;  }
    | ProcFuncList ProcDecl
    {
        ((struct AstNode *) $2)->next = (struct AstNode *) $1;
        $$ = $2;
    }
    | ProcFuncList FuncDecl
    {
        ((struct AstNode *) $2)->next = (struct AstNode *) $1;
        $$ = $2;
    }
    ;

ProcDecl:
    T_PROCEDURE Identifier T_LPAR ParamList T_RPAR T_SEMICOLON VarDeclList
    CodeBlock T_SEMICOLON
    {
        struct AstNode *ast_node;
        ast_node = ast_node_new("ProcDecl", -1, PROCEDURE,
                                yylloc.last_line, NULL);
        ast_node->children[0] = $2; // Identifier
        ast_node->children[1] = $4; // ParamList
        ast_node->children[2] = $7; // VarDeclList
        ast_node->children[3] = $8; // CodeBlock
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
        ast_node->children[0] = $2; // Identifier
        ast_node->children[1] = $4; // ParamList
        ast_node->children[2] = $7; // SimpleType
        ast_node->children[3] = $9; // VarDeclList
        ast_node->children[4] = $10;// CodeBlock
        $$ = ast_node;
    }
    ;

ParamList:
    /* empty */ { $$ = NULL; }
    | SingleParam MultiParam
    {
        ((struct AstNode *) $1)->next = (struct AstNode *) $2;
        $$ = $1;
    }
    ;

SingleParam:
    Identifier T_COLON SimpleType
    {
        struct AstNode *ast_node;
        ast_node = ast_node_new("SingleParam", -1, -1,
                                yylloc.last_line, NULL);
        ast_node->children[0] = $1;
        ast_node->children[1] = $3;
        $$ = ast_node;
    }
    ;

MultiParam:
    /* empty */ { $$ = NULL; }
    | T_COMMA SingleParam MultiParam
    {
        ((struct AstNode *) $2)->next = (struct AstNode *) $3;
        $$ = $2;
    }
    ;

MainCodeBlock:
    /* empty */ { $$ = NULL; }
    | CodeBlock T_DOT { $$ = $1; }
    ;

CodeBlock:
    T_BEGIN Assignment T_END
    {
        $$ = $2;
    }
    ;

Assignment:
    Identifier T_ASSIGNMENT Literal T_SEMICOLON
    {
        struct AstNode *ast_node;
        ast_node = ast_node_new("Assignment", -1, -1,
                                yylloc.last_line, NULL);
        ast_node->children[0] = $1;
        ast_node->children[1] = $3;
        //ast_node_print($3);
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

