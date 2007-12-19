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
%option case-insensitive

%%
"var"                       { uploc; return T_VAR; }
"integer"|"boolean"|"char"  { uploc;
                              yylval->type = type_get_from_lexeme(yytext);
                              return TYPE_IDENTIFIER;
                            }

"procedure"                 { uploc; return T_PROCEDURE; }
"function"                  { uploc; return T_FUNCTION; }
"begin"                     { uploc; return T_BEGIN; }
"end"                       { uploc; return T_END; }

"printInt"                  { uploc; return T_PRINT_INT; }
"printBoolean"              { uploc; return T_PRINT_BOOL; }
"printChar"                 { uploc; return T_PRINT_CHAR; }
"println"                   { uploc; return T_PRINT_LINE; }

"if"                        { uploc; return T_IF; }
"then"                      { uploc; return T_THEN; }
"else"                      { uploc; return T_ELSE; }
"while"                     { uploc; return T_WHILE; }
"for"                       { uploc; return T_FOR; }
"to"                        { uploc; return T_TO; }
"do"                        { uploc; return T_DO; }

":="                        { uploc; return T_ASSIGNMENT; }
"("                         { uploc; return T_LPAR; }
")"                         { uploc; return T_RPAR; }
"+"                         { uploc; return T_PLUS; }
"-"                         { uploc; return T_MINUS; }
"*"                         { uploc; return T_STAR; }
"/"                         { uploc; return T_SLASH; }
"and"                       { uploc; return T_AND; }
"or"                        { uploc; return T_OR; }
"not"                       { uploc; return T_NOT; }

">"                         { uploc; return T_GREATER; }
"<"                         { uploc; return T_LESSER; }
"="                         { uploc; return T_EQUAL; }
"<>"                        { uploc; return T_NOTEQUAL; }
">="                        { uploc; return T_GREATEREQUAL; }
"<="                        { uploc; return T_LESSEREQUAL; }

";"                         { uploc; return T_SEMICOLON; }
":"                         { uploc; return T_COLON; }
"."                         { uploc; return T_DOT; }
","                         { uploc; return T_COMMA; }

"true"|"false"              { uploc;
                              yylval->boolean=strcmp(yytext, "False");
                              return BOOL_LITERAL;
                            }
"'"."'"                     { uploc;
                              yylval->character=yytext[1];
                              return CHAR_LITERAL;
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

