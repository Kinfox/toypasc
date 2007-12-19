%{
#include <stdio.h>
#include "base.h"
#include "parser.h"

#define uploc   { yylloc->first_column = yylloc->last_column + 1; yylloc->last_column += yyleng; }
/*#define YY_DECL int yylex (YYSTYPE *yylval_param)*/
%}

%option yylineno
%option bison-bridge
%option bison-locations
%option noyywrap
%option nounput

%%
"Var"                       { uploc; return T_VAR; }
"Integer"|"Boolean"|"Char"  { uploc;
                              yylval->type = type_get_from_lexeme(yytext);
                              return TYPE_IDENTIFIER;
                            }

"Procedure"                 { uploc; return T_PROCEDURE; }
"Function"                  { uploc; return T_FUNCTION; }
"Begin"                     { uploc; return T_BEGIN; }
"End"                       { uploc; return T_END; }

"printInt"                  { uploc; return T_PRINT_INT; }
"printBoolean"              { uploc; return T_PRINT_BOOL; }
"printChar"                 { uploc; return T_PRINT_CHAR; }
"println"                   { uploc; return T_PRINT_LINE; }

"if"                        { uploc; return T_IF; }
"else"                      { uploc; return T_ELSE; }
"while"                     { uploc; return T_WHILE; }
"for"                       { uploc; return T_FOR; }
"to"                        { uploc; return T_TO; }
"do"                        { uploc; return T_DO; }

":="                        { uploc; return T_ASSIGNMENT; }
"("                         { uploc; return T_LPAR; }
")"                         { uploc; return T_RPAR; }
"+"                         { uploc; return T_ADD; }
"-"                         { uploc; return T_SUB; }
"*"                         { uploc; return T_MUL; }
"/"                         { uploc; return T_DIV; }
"and"                       { uploc; return T_AND; }
"or"                        { uploc; return T_OR; }
"not"                       { uploc; return T_NOT; }

">"                         { uploc; return T_MAJOR; }
"<"                         { uploc; return T_MINOR; }
"="                         { uploc; return T_EQUAL; }
"<>"                        { uploc; return T_NOTEQUAL; }
">="                        { uploc; return T_MAJOREQUAL; }
"<="                        { uploc; return T_MINOREQUAL; }

";"                         { uploc; return T_SEMICOLON; }
":"                         { uploc; return T_COLON; }
"."                         { uploc; return T_DOT; }
","                         { uploc; return T_COMMA; }

"True"|"False"              { uploc;
                              yylval->boolean=strcmp(yytext, "False");
                              return BOOL_LITERAL;
                            }
[0-9]+                      { uploc;
                              yylval->integer=atoi(yytext);
                              return INT_LITERAL;
                            }
[A-Za-z][A-Za-z0-9]*        { uploc;
                              yylval->lexeme=strdup(yytext);
                              return IDENTIFIER;
                            }

[ \t]+                      /* ignora whitespace */;
[\n]                        { yylloc->first_line = yylloc->last_line = yylineno;
                              yylloc->first_column = 1;
                              yylloc->last_column = 0;
                            }

.                           { uploc;
                              fprintf (stderr,
                                       "%d: syntax error: unexpected character ´%s´\n",
                                       yylloc->first_line, yytext);
                            }
%%

