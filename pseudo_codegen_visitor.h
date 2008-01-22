#ifndef PSEUDO_CODEGEN_VISITOR_H
#define PSEUDO_CODEGEN_VISITOR_H

#include "ast.h"

Visitor *pseudo_codegen_new();

void pseudo_codegen_visit_program (struct AstNode *node);
void pseudo_codegen_visit_programdecl (struct AstNode *node);
void pseudo_codegen_visit_vardecl_list (struct AstNode *node);
void pseudo_codegen_visit_identifier_list (struct AstNode *node);
void pseudo_codegen_visit_procfunc_list (struct AstNode *node);
void pseudo_codegen_visit_procfunc (struct AstNode *node);
void pseudo_codegen_visit_param_list (struct AstNode *node);
void pseudo_codegen_visit_statement_list (struct AstNode *node);
void pseudo_codegen_visit_binary_expr (struct AstNode *node);
void pseudo_codegen_visit_callparam_list (struct AstNode *node);
void pseudo_codegen_visit_identifier (struct AstNode *node);
void pseudo_codegen_visit_literal (struct AstNode *node);

void pseudo_codegen_visit_simplenode (struct AstNode *node);
void pseudo_codegen_close_group ();

#endif // PSEUDO_CODEGEN_VISITOR_H
