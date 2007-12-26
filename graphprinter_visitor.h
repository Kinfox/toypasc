#ifndef GRAPHPRINTER_VISITOR_H
#define GRAPHPRINTER_VISITOR_H

#include "ast.h"

Visitor *graphprinter_new();

void graphprinter_visit_program (struct AstNode *node);
void graphprinter_visit_programdecl (struct AstNode *node);
void graphprinter_visit_vardecl_list (struct AstNode *node);
void graphprinter_visit_identifier_list (struct AstNode *node);
void graphprinter_visit_procfunc_list (struct AstNode *node);
void graphprinter_visit_procfunc (struct AstNode *node);
void graphprinter_visit_param_list (struct AstNode *node);
void graphprinter_visit_statement_list (struct AstNode *node);
void graphprinter_visit_binary_expr (struct AstNode *node);
void graphprinter_visit_callparam_list (struct AstNode *node);
void graphprinter_visit_identifier (struct AstNode *node);
void graphprinter_visit_literal (struct AstNode *node);

void graphprinter_visit_simplenode (struct AstNode *node);
void graphprinter_close_group ();

#endif // GRAPHPRINTER_VISITOR_H
