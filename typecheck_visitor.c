#include <stdlib.h>
#include <stdio.h>
#include "typecheck_visitor.h"

Visitor *
typecheck_visitor_new()
{
    Visitor *tc_visitor = (Visitor *) malloc (sizeof(Visitor));

    tc_visitor->visit_program = &typecheck_visit_program;
    tc_visitor->visit_programdecl = &typecheck_visit_programdecl;
    tc_visitor->visit_vardecl_list = &typecheck_visit_vardecl_list;
    tc_visitor->visit_vardecl = &typecheck_visit_vardecl;
    tc_visitor->visit_identifier_list = &typecheck_visit_identifier_list;
    tc_visitor->visit_procfunc_list = &typecheck_visit_procfunc_list;
    tc_visitor->visit_procedure = &typecheck_visit_procedure;
    tc_visitor->visit_function = &typecheck_visit_function;
    tc_visitor->visit_param_list = &typecheck_visit_param_list;
    tc_visitor->visit_parameter = &typecheck_visit_parameter;
    tc_visitor->visit_statement_list = &typecheck_visit_statement_list;
    tc_visitor->visit_printint_stmt = &typecheck_visit_printint_stmt;
    tc_visitor->visit_printchar_stmt = &typecheck_visit_printchar_stmt;
    tc_visitor->visit_printbool_stmt = &typecheck_visit_printbool_stmt;
    tc_visitor->visit_printline_stmt = &typecheck_visit_printline_stmt;
    tc_visitor->visit_assignment_stmt = &typecheck_visit_assignment_stmt;
    tc_visitor->visit_if_stmt = &typecheck_visit_if_stmt;
    tc_visitor->visit_while_stmt = &typecheck_visit_while_stmt;
    tc_visitor->visit_for_stmt = &typecheck_visit_for_stmt;
    tc_visitor->visit_rel_expr = &typecheck_visit_rel_expr;
    tc_visitor->visit_add_expr = &typecheck_visit_add_expr;
    tc_visitor->visit_mul_expr = &typecheck_visit_mul_expr;
    tc_visitor->visit_notfactor = &typecheck_visit_notfactor;
    tc_visitor->visit_call = &typecheck_visit_call;
    tc_visitor->visit_callparam_list = &typecheck_visit_callparam_list;
    tc_visitor->visit_identifier = &typecheck_visit_identifier;
    tc_visitor->visit_literal = &typecheck_visit_literal;
}

void
typecheck_visit_program(struct AstNode *node)
{
    printf("typecheck_visit_program\n");
}

void
typecheck_visit_programdecl(struct AstNode *node)
{
    printf("typecheck_visit_program_declaration\n");
}

void
typecheck_visit_vardecl_list (struct AstNode *node)
{
    printf("typecheck_visit_vardecl_list\n");
}

void
typecheck_visit_vardecl (struct AstNode *node)
{
    printf("typecheck_visit_vardecl\n");
}

void
typecheck_visit_identifier_list (struct AstNode *node)
{
    printf("typecheck_visit_identifier_list\n");
}

void
typecheck_visit_procfunc_list (struct AstNode *node)
{
    printf("typecheck_visit_procfunc_list\n");
}

void
typecheck_visit_procedure (struct AstNode *node)
{
    printf("typecheck_visit_procedure\n");
}

void
typecheck_visit_function (struct AstNode *node)
{
    printf("typecheck_visit_function\n");
}

void
typecheck_visit_param_list (struct AstNode *node)
{
    printf("typecheck_visit_param_list\n");
}

void
typecheck_visit_parameter (struct AstNode *node)
{
    printf("typecheck_visit_parameter\n");
}

void
typecheck_visit_statement_list (struct AstNode *node)
{
    printf("typecheck_visit_statement_list\n");
}

void
typecheck_visit_printint_stmt (struct AstNode *node)
{
}

void
typecheck_visit_printchar_stmt (struct AstNode *node)
{
}

void
typecheck_visit_printbool_stmt (struct AstNode *node)
{
}

void
typecheck_visit_printline_stmt (struct AstNode *node)
{
}

void
typecheck_visit_assignment_stmt (struct AstNode *node)
{
}

void
typecheck_visit_if_stmt (struct AstNode *node)
{
}

void
typecheck_visit_while_stmt (struct AstNode *node)
{
}

void
typecheck_visit_for_stmt (struct AstNode *node)
{
}

void
typecheck_visit_rel_expr (struct AstNode *node)
{
}

void
typecheck_visit_add_expr (struct AstNode *node)
{
}

void
typecheck_visit_mul_expr (struct AstNode *node)
{
}

void
typecheck_visit_notfactor (struct AstNode *node)
{
}

void
typecheck_visit_call (struct AstNode *node)
{
}

void
typecheck_visit_callparam_list (struct AstNode *node)
{
}

void
typecheck_visit_identifier (struct AstNode *node)
{
}

void
typecheck_visit_literal (struct AstNode *node)
{
}


