#include <stdlib.h>
#include <stdio.h>
#include "typecheck_visitor.h"

static bool is_vardecl = FALSE;
static Symbol *procfunc_symbol = NULL;
static Symbol *_symbol_lookup(Symbol *sym);
static Type _get_expression_type(struct AstNode *node);

Visitor *
typecheck_new()
{
    Visitor *tc_visitor = (Visitor *) malloc (sizeof(Visitor));

    tc_visitor->visit_program = &typecheck_visit_program;
    tc_visitor->visit_programdecl = &typecheck_visit_programdecl;
    tc_visitor->visit_vardecl_list = &typecheck_visit_vardecl_list;
    tc_visitor->visit_vardecl = &typecheck_visit_vardecl;
    tc_visitor->visit_identifier_list = &typecheck_visit_donothing;
    tc_visitor->visit_procfunc_list = &typecheck_visit_procfunc_list;
    tc_visitor->visit_procedure = &typecheck_visit_procfunc;
    tc_visitor->visit_function = &typecheck_visit_procfunc;
    tc_visitor->visit_param_list = &typecheck_visit_donothing;
    tc_visitor->visit_parameter = &typecheck_visit_parameter;
    tc_visitor->visit_statement_list = &typecheck_visit_statement_list;
    tc_visitor->visit_print_stmt = &typecheck_visit_print_stmt;
    tc_visitor->visit_assignment_stmt = &typecheck_visit_assignment_stmt;
    tc_visitor->visit_if_stmt = &typecheck_visit_if_stmt;
    tc_visitor->visit_while_stmt = &typecheck_visit_while_stmt;
    tc_visitor->visit_for_stmt = &typecheck_visit_for_stmt;
    tc_visitor->visit_rel_expr = &typecheck_visit_binary_expr;
    tc_visitor->visit_add_expr = &typecheck_visit_binary_expr;
    tc_visitor->visit_mul_expr = &typecheck_visit_binary_expr;
    tc_visitor->visit_notfactor = &typecheck_visit_notfactor;
    tc_visitor->visit_call = &typecheck_visit_call;
    tc_visitor->visit_callparam_list = &typecheck_visit_callparam_list;
    tc_visitor->visit_identifier = &typecheck_visit_identifier;
    tc_visitor->visit_literal = &typecheck_visit_donothing;
    tc_visitor->close_group = NULL;
}

void
typecheck_visit_donothing(struct AstNode *node)
{
}

void
typecheck_visit_program(struct AstNode *node)
{
    node->symbol = symbol_new(NULL);
    global_symtab = node->symbol;
    symtab = global_symtab;
}

void
typecheck_visit_programdecl(struct AstNode *node)
{
    is_vardecl = TRUE;
}

void
typecheck_visit_procfunc_list(struct AstNode *node)
{
    symtab = global_symtab;
}

void
typecheck_visit_procfunc(struct AstNode *node)
{
    is_vardecl = FALSE;
    symtab = node->symbol;
    procfunc_symbol = node->children->symbol;
}

void
typecheck_visit_vardecl_list (struct AstNode *node)
{
    is_vardecl = TRUE;
    symtab = node->parent->symbol;
}

void
typecheck_visit_vardecl (struct AstNode *node)
{
    declared_type = node->type;
}

void
typecheck_visit_parameter (struct AstNode *node)
{
    is_vardecl = TRUE;
    declared_type = node->type;
}

void
typecheck_visit_statement_list(struct AstNode *node)
{
    is_vardecl = FALSE;
    declared_type = VOID;
    symtab = node->parent->symbol;
}

void
typecheck_visit_print_stmt (struct AstNode *node)
{
}

void
typecheck_visit_assignment_stmt (struct AstNode *node)
{
    struct AstNode *node1 = node->children;
    struct AstNode *node2 = node1->sibling;

    if (node1->kind != IDENTIFIER) {
        fprintf(stderr,
                "Error: Left side of assignment must be an Identifier. Check line %d.\n",
                node->linenum);
        return;
    }

    if (_get_expression_type(node1) != _get_expression_type(node2)) {
        node->type = ERROR;
        fprintf(stderr,
                "Error: Incompatible types on assignment operation in line %d.\n",
                node->linenum);
    }
}

void
typecheck_visit_if_stmt (struct AstNode *node)
{
    struct AstNode *node1 = node->children;
    struct AstNode *node2 = node1->sibling;
    struct AstNode *node3 = node2->sibling;

    /*if (_get_expression_type(node1) != BOOLEAN) {
        node->type = ERROR;
        fprintf(stderr,
                "Error: Condition for if statement must return a boolean. Check line %d.\n",
                node->linenum);
    }*/
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
typecheck_visit_binary_expr (struct AstNode *node)
{
    Type type1;
    Type type2;
    Symbol *symbol;
    struct AstNode *node1 = node->children;
    struct AstNode *operator = node1->sibling;
    struct AstNode *node2 = operator->sibling;

    type1 = _get_expression_type(node1);
    type2 = _get_expression_type(node2);

    if (type1 != type2) {
        node->type = ERROR;
        fprintf(stderr,
                "Error: Operation '%s' over incompatible types on line %d.\n",
                operator->name, operator->linenum);
    }
}

void
typecheck_visit_notfactor (struct AstNode *node)
{
}

void
typecheck_visit_call (struct AstNode *node)
{
    int i;
    char *name;
    Symbol *sym;

    name = node->children->symbol->name;
    sym = symbol_lookup(global_symtab, name);
    procfunc_symbol = sym;

    if (sym == NULL) {
        fprintf(stderr,
                "Error: Undeclared function or procedure '%s' in line %d\n",
                name, node->linenum);
        return;
    }

    node->type = sym->type;
}

void
typecheck_visit_callparam_list (struct AstNode *node)
{
    int i;
    struct AstNode *temp;

    i = 0;
    for (temp = node->children; temp != NULL; temp = temp->sibling) {
        if (temp->type != procfunc_symbol->param_types[i]) {
            fprintf(stderr, "Error: Call '%s' on line %d, expecting %s, received %s.\n",
                    procfunc_symbol->name, node->linenum,
                    type_get_lexeme(procfunc_symbol->param_types[i]),
                    type_get_lexeme(temp->type));
        }

        i++;
        if (i > procfunc_symbol->params)
            break;
    }
    if (i != procfunc_symbol->params) {
        fprintf(stderr, "Error: Expecting %d parameters, received %d.\n",
                i, procfunc_symbol->params);
    }
}

void
typecheck_visit_identifier (struct AstNode *node)
{
    Symbol *sym;

    if (is_vardecl) {
        node->type = declared_type;
        node->symbol->type = declared_type;
        sym = symbol_insert(symtab, node->symbol);

        if (node->parent->kind == PARAMETER) {
            procfunc_symbol->param_types[procfunc_symbol->params] = node->symbol->type;
            procfunc_symbol->params++;
        }

        if (sym != node->symbol)
            fprintf(stderr, "Error: Symbol '%s' already defined in line XX\n",
                    sym->name);

    } else if (node->parent->kind == FUNCTION ||
               node->parent->kind == PROCEDURE) {
        sym = symbol_insert(global_symtab, node->symbol);
        node->type = node->symbol->type;
        node->parent->type = node->type;
        procfunc_symbol = node->symbol;

        if (sym != node->symbol)
            fprintf(stderr, "Error: Symbol '%s' already defined in line XX\n",
                    sym->name);

    } else {
        sym = _symbol_lookup(node->symbol);

        if (sym != NULL)
            node->symbol = sym;
        else {
            node->symbol->decl_linenum = 0;
            fprintf(stderr, "Error: Undeclared symbol '%s' in line %d\n",
                    node->symbol->name, node->linenum);
        }

        if (node->parent->kind == CALLPARAM_LIST) {
            int i;
            //procfunc_symbol->param_types[procfunc_symbol->params] = node->symbol->type;
            //procfunc_symbol->params++;
            fprintf(stderr, "Parameter: %s (%x)\n",
                    procfunc_symbol->name, procfunc_symbol);
            fprintf(stderr, "types (%d): ", procfunc_symbol->params);
            for (i = 0; i < procfunc_symbol->params; i++) {
                fprintf(stderr, "%d ",
                        procfunc_symbol->param_types[procfunc_symbol->params]);
            }
            fprintf(stderr, "\n");
        }
    }

}

static Symbol *
_symbol_lookup(Symbol *sym)
{
    Symbol *symbol = NULL;

    symbol = symbol_lookup(symtab, sym->name);

    if (symbol == NULL)
        symbol = symbol_lookup(global_symtab, sym->name);

    return symbol;
}

static Type
_get_expression_type(struct AstNode *node)
{
    char *name;
    Symbol *sym;

    switch (node->kind) {
        case IDENTIFIER:
            sym = _symbol_lookup(node->symbol);
            if (sym != NULL) {
                node->type = sym->type;
                return sym->type;
            }
            break;

        case INT_LITERAL:
        case BOOL_LITERAL:
        case CHAR_LITERAL:
            return node->type;

        case NOTFACTOR:
            return BOOLEAN;

        case ADD_EXPR:
        case MUL_EXPR:
        case REL_EXPR:
            return INTEGER;

        case CALL:
            name = node->children->symbol->name;
            sym = symbol_lookup(global_symtab, name);
            if (sym != NULL) {
               node->type = sym->type;
               return sym->type;
            }
            break;
    }

    return VOID;
}

/*static Type
_get_func_type(struct AstNode *node)
{
    Symbol *symbol;
    char *name;

    if (node->kind != FUNCTION)
        return VOID;

    name = node->children->symbol->name;
    symbol = symbol_lookup(global_symtab, name);

    if (symbol == NULL) {
        fprintf(stderr, "Error: Undeclared symbol '%s' in line %d\n",
                name, node->linenum);
        return VOID;
    }

    return symbol->type;
}*/
