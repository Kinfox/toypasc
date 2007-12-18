%{
#include <stdio.h>
#include <string.h>
#include "parser.h"
#include "base.h"
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
}

%token T_INTEGER
%token T_BOOLEAN

%token T_VAR
%token T_BEGIN
%token T_END
%token T_PROCEDURE
%token T_FUNCTION

%token T_PRINT_INT
%token T_PRINT_BOOL
%token T_PRINT_LINE

%token T_ASSIGNMENT

%token T_ADD
%token T_SUB
%token T_DIV
%token T_MULT

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

%token <lexeme> IDENTIFIER
%token <integer> NUMBER
%token <boolean> BOOLEAN

/*%type <symbol> program
%type <symbol> var_decl_list
%type <symbol> var_decl
%type <symbol> var_type
%type <symbol> proc_func_list
%type <symbol> proc_func_decl
%type <symbol> proc_func_body
%type <symbol> proc_decl
%type <symbol> func_decl
%type <symbol> param_list
%type <symbol> single_param
%type <symbol> multi_param
%type <symbol> code_block
%type <symbol> statements
%type <symbol> statement*/

%type <lexeme> assignment
%type <integer> numeric_expression
%type <boolean> relational_expression
%type <integer> number_literal

/*%type <symbol> print_statement
%type <symbol> print_integer
%type <symbol> print_boolean
%type <symbol> print_line*/

%start program

%%
program:
    var_decl_list proc_func_list code_block T_DOT { printf("program\n"); }
    ;

var_decl_list:
    var_decl_list var_decl { printf("\n"); }
    | /* empty */
    ;

var_decl:
    T_VAR IDENTIFIER T_COLON var_type T_SEMICOLON
    { printf("var_decl\n") }
    ;

var_type:
    T_INTEGER
    { printf("var_type\n") }
    | T_BOOLEAN
    { printf("var_type\n") }
    ;

proc_func_list:
    /* empty */
    { printf("proc_func_list\n") }
    | proc_func_list proc_decl
    { printf("proc_func_list\n") }
    | proc_func_list func_decl
    { printf("proc_func_list\n") }
    ;

proc_func_decl:
    IDENTIFIER T_LPAR param_list T_RPAR
    { printf("proc_func_decl\n") }
    ;

proc_func_body:
    var_decl_list code_block T_SEMICOLON
    { printf("proc_func_body\n") }
    ;

proc_decl:
    T_PROCEDURE proc_func_decl T_SEMICOLON proc_func_body
    { printf("proc_decl\n") }
    ;

func_decl:
    T_FUNCTION proc_func_decl T_COLON var_type T_SEMICOLON proc_func_body
    { printf("func_decl\n") }
    ;

param_list:
    /* empty */
    { printf("param_list\n") }
    | single_param multi_param
    { printf("param_list\n") }
    ;

single_param:
    IDENTIFIER T_COLON var_type
    { printf("single_param\n") }
    ;

multi_param:
    /* empty */
    { printf("multi_param\n") }
    | T_COMMA single_param multi_param
    { printf("multi_param\n") }
    ;

code_block:
    T_BEGIN statements T_END
    { printf("code_block\n") }
    ;

statements:
    /* empty */
    | statements statement T_SEMICOLON
    ;

statement:
    assignment
    | print_statement
    ;

assignment:
    IDENTIFIER T_ASSIGNMENT expression
    | IDENTIFIER T_ASSIGNMENT NUMBER { $$ = $1; }
    ;

expression:
    numeric_expression
    | relational_expression
    ;

numeric_expression:
    numeric_expression T_ADD numeric_expression { $$ = $1 + $3; }
    | numeric_expression T_SUB numeric_expression { $$ = $1 - $3; }
    | number_literal { $$ = $1; }
    ;

number_literal:
    NUMBER { $$ = $1; }
    ;

relational_expression:
    numeric_expression relational_operator numeric_expression
    ;

relational_operator:
    T_MINOR
    | T_MINOREQUAL
    | T_MAJOR
    | T_MAJOREQUAL
    | T_EQUAL
    | T_NOTEQUAL
    ;

print_statement:
    print_integer
    | print_boolean
    | print_line
    ;

print_integer:
    T_PRINT_INT T_LPAR NUMBER T_RPAR { printf ("%d", $3); }

print_boolean:
    T_PRINT_BOOL T_LPAR BOOLEAN T_RPAR { printf ("%s", ($3 == 0 ? "False" : "True")); }

print_line:
    T_PRINT_LINE T_LPAR T_RPAR { printf ("\n"); }
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

    return yyparse();
}

