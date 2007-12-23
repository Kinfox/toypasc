#ifndef TYPECHECK_VISITOR_H
#define TYPECHECK_VISITOR_H

#include "ast.h"

Visitor *typecheck_visitor_new();

void typecheck_visit_program (struct AstNode *node);
void typecheck_visit_programdecl (struct AstNode *node);
void typecheck_visit_vardecl_list (struct AstNode *node);
void typecheck_visit_vardecl (struct AstNode *node);
void typecheck_visit_identifier_list (struct AstNode *node);
void typecheck_visit_procfunc_list (struct AstNode *node);
void typecheck_visit_procedure (struct AstNode *node);
void typecheck_visit_function (struct AstNode *node);
void typecheck_visit_param_list (struct AstNode *node);
void typecheck_visit_parameter (struct AstNode *node);
void typecheck_visit_statement_list (struct AstNode *node);
void typecheck_visit_printint_stmt (struct AstNode *node);
void typecheck_visit_printchar_stmt (struct AstNode *node);
void typecheck_visit_printbool_stmt (struct AstNode *node);
void typecheck_visit_printline_stmt (struct AstNode *node);
void typecheck_visit_assignment_stmt (struct AstNode *node);
void typecheck_visit_if_stmt (struct AstNode *node);
void typecheck_visit_while_stmt (struct AstNode *node);
void typecheck_visit_for_stmt (struct AstNode *node);
void typecheck_visit_rel_expr (struct AstNode *node);
void typecheck_visit_add_expr (struct AstNode *node);
void typecheck_visit_mul_expr (struct AstNode *node);
void typecheck_visit_notfactor (struct AstNode *node);
void typecheck_visit_call (struct AstNode *node);
void typecheck_visit_callparam_list (struct AstNode *node);
void typecheck_visit_identifier (struct AstNode *node);
void typecheck_visit_literal (struct AstNode *node);

#endif // TYPECHECK_VISITOR_H
