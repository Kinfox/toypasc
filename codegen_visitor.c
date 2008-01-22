#include <stdlib.h>
#include <stdio.h>
#include "codegen_visitor.h"

static bool is_vardecl = FALSE;
static Symbol *procfunc_symbol = NULL;
static Symbol *_symbol_lookup(Symbol *sym);
static Type _get_expression_type(struct AstNode *node);

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
    node->symbol = symbol_new(NULL);
    global_symtab = node->symbol;
    symtab = global_symtab;
}

void
codegen_visit_programdecl(struct AstNode *node)
{
    is_vardecl = TRUE;
}

void
codegen_visit_procfunc_list(struct AstNode *node)
{
    symtab = global_symtab;
}

void
codegen_visit_procfunc(struct AstNode *node)
{
    is_vardecl = FALSE;
    symtab = node->symbol;
    procfunc_symbol = node->children->symbol;
}

void
codegen_visit_vardecl_list (struct AstNode *node)
{
    is_vardecl = TRUE;
    symtab = node->parent->symbol;
}

void
codegen_visit_vardecl (struct AstNode *node)
{
    declared_type = node->type;
}

void
codegen_visit_parameter (struct AstNode *node)
{
    is_vardecl = TRUE;
    declared_type = node->type;
}

void
codegen_visit_statement_list(struct AstNode *node)
{
    is_vardecl = FALSE;
    declared_type = VOID;
    symtab = node->parent->symbol;
}

void
codegen_visit_print_stmt (struct AstNode *node)
{
}

void
codegen_visit_assignment_stmt (struct AstNode *node)
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
codegen_visit_if_stmt (struct AstNode *node)
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
codegen_visit_while_stmt (struct AstNode *node)
{
}

void
codegen_visit_for_stmt (struct AstNode *node)
{
    Type type;
    struct AstNode *assign_node = node->children;
    struct AstNode *id_node = assign_node->children;
    struct AstNode *expr1_node = id_node->sibling;
    struct AstNode *expr2_node = assign_node->sibling;

    type = _get_expression_type(id_node);
    if (type != INTEGER) {
        node->type = ERROR;
        fprintf(stderr,
                "Error: Identifier '%s' is not of Integer type. "
                "Check line %d.\n",
                id_node->symbol->name, id_node->linenum);
    }
    type = _get_expression_type(expr1_node);
    if (type != INTEGER) {
        node->type = ERROR;
        fprintf(stderr,
                "Error: Initial value of '%s' is not of Integer type. "
                "Check line %d.\n",
                id_node->symbol->name, expr1_node->linenum);
    }
    type = _get_expression_type(expr1_node);
    if (type != INTEGER) {
        node->type = ERROR;
        fprintf(stderr,
                "Error: Value of stop condition is not of Integer type. "
                "Check line %d.\n", expr2_node->linenum);
    }
}

void
codegen_visit_binary_expr (struct AstNode *node)
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
codegen_visit_notfactor (struct AstNode *node)
{
}

void
codegen_visit_call (struct AstNode *node)
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
codegen_visit_callparam_list (struct AstNode *node)
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
codegen_visit_identifier (struct AstNode *node)
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

    swicgh (node->kind) {
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