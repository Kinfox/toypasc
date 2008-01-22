#ifndef CODEGEN_VISITOR_H
#define CODEGEN_VISITOR_H

#include "ast.h"
#include "base.h"
#include "symbol_table.h"

Visitor *codegen_new();

void codegen_visit_program (struct AstNode *node);
void codegen_visit_programdecl (struct AstNode *node);
void codegen_visit_procfunc_list (struct AstNode *node);
void codegen_visit_procfunc(struct AstNode *node);
void codegen_visit_vardecl_list (struct AstNode *node);
void codegen_visit_vardecl (struct AstNode *node);
void codegen_visit_parameter (struct AstNode *node);
void codegen_visit_statement_list(struct AstNode *node);
void codegen_visit_printint_stmt (struct AstNode *node);
void codegen_visit_printchar_stmt (struct AstNode *node);
void codegen_visit_printbool_stmt (struct AstNode *node);
void codegen_visit_printline_stmt (struct AstNode *node);
void codegen_visit_assignment_stmt (struct AstNode *node);
void codegen_visit_if_stmt (struct AstNode *node);
void codegen_visit_while_stmt (struct AstNode *node);
void codegen_visit_for_stmt (struct AstNode *node);
void codegen_visit_binary_expr (struct AstNode *node);
void codegen_visit_notfactor (struct AstNode *node);
void codegen_visit_call (struct AstNode *node);
void codegen_visit_callparam_list (struct AstNode *node);
void codegen_visit_identifier (struct AstNode *node);

void codegen_visit_donothing(struct AstNode *node);

#endif // codegen_VISITOR_H
