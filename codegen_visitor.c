#include <stdlib.h>
#include <stdio.h>
#include "codegen_visitor.h"

Visitor *
codegen_new()
{
    Visitor *cg_visitor = (Visitor *) malloc (sizeof(Visitor));

    cg_visitor->visit_program = &codegen_visit_program;
    cg_visitor->visit_programdecl = &codegen_visit_programdecl;
    cg_visitor->visit_vardecl_list = &codegen_visit_vardecl_list;
    cg_visitor->visit_vardecl = &codegen_visit_vardecl;
    cg_visitor->visit_identifier_list = &codegen_visit_donothing;
    cg_visitor->visit_procfunc_list = &codegen_visit_procfunc_list;
    cg_visitor->visit_procedure = &codegen_visit_procfunc;
    cg_visitor->visit_function = &codegen_visit_procfunc;
    cg_visitor->visit_param_list = &codegen_visit_donothing;
    cg_visitor->visit_parameter = &codegen_visit_parameter;
    cg_visitor->visit_statement_list = &codegen_visit_statement_list;
    cg_visitor->visit_print_stmt = &codegen_visit_print_stmt;
    cg_visitor->visit_assignment_stmt = &codegen_visit_assignment_stmt;
    cg_visitor->visit_if_stmt = &codegen_visit_if_stmt;
    cg_visitor->visit_while_stmt = &codegen_visit_while_stmt;
    cg_visitor->visit_for_stmt = &codegen_visit_for_stmt;
    cg_visitor->visit_rel_expr = &codegen_visit_binary_expr;
    cg_visitor->visit_add_expr = &codegen_visit_binary_expr;
    cg_visitor->visit_mul_expr = &codegen_visit_binary_expr;
    cg_visitor->visit_notfactor = &codegen_visit_notfactor;
    cg_visitor->visit_call = &codegen_visit_call;
    cg_visitor->visit_callparam_list = &codegen_visit_callparam_list;
    cg_visitor->visit_identifier = &codegen_visit_identifier;
    cg_visitor->visit_literal = &codegen_visit_donothing;
    cg_visitor->close_group = NULL;
}

void
codegen_visit_donothing(struct AstNode *node)
{
}

void
codegen_visit_program(struct AstNode *node)
{
}

void
codegen_visit_programdecl(struct AstNode *node)
{
}

void
codegen_visit_procfunc_list(struct AstNode *node)
{
}

void
codegen_visit_procfunc(struct AstNode *node)
{
}

void
codegen_visit_vardecl_list (struct AstNode *node)
{
}

void
codegen_visit_vardecl (struct AstNode *node)
{
}

void
codegen_visit_parameter (struct AstNode *node)
{
}

void
codegen_visit_statement_list(struct AstNode *node)
{
}

void
codegen_visit_printint_stmt (struct AstNode *node)
{
}

void
codegen_visit_printchar_stmt (struct AstNode *node)
{
}

void
codegen_visit_printbool_stmt (struct AstNode *node)
{
}

void
codegen_visit_printline_stmt (struct AstNode *node)
{
}

void
codegen_visit_assignment_stmt (struct AstNode *node)
{
}

void
codegen_visit_if_stmt (struct AstNode *node)
{
}

void
codegen_visit_while_stmt (struct AstNode *node)
{
}

void
codegen_visit_for_stmt (struct AstNode *node)
{
}

void
codegen_visit_binary_expr (struct AstNode *node)
{
}

void
codegen_visit_notfactor (struct AstNode *node)
{
}

void
codegen_visit_call (struct AstNode *node)
{
}

void
codegen_visit_callparam_list (struct AstNode *node)
{
}

void
codegen_visit_identifier (struct AstNode *node)
{
}

