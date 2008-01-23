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
    Visitor *visitor = (Visitor *) malloc (sizeof(Visitor));

    visitor->visit_program = &typecheck_visit_program;
    visitor->visit_programdecl = &typecheck_visit_programdecl;
    visitor->visit_vardecl_list = &typecheck_visit_vardecl_list;
    visitor->visit_vardecl = &typecheck_visit_vardecl;
    visitor->visit_identifier_list = &typecheck_visit_pass;
    visitor->visit_procfunc_list = &typecheck_visit_procfunc_list;
    visitor->visit_procedure = &typecheck_visit_procfunc;
    visitor->visit_function = &typecheck_visit_procfunc;
    visitor->visit_param_list = &typecheck_visit_pass;
    visitor->visit_parameter = &typecheck_visit_parameter;
    visitor->visit_statement_list = &typecheck_visit_statement_list;
    visitor->visit_printint_stmt = &typecheck_visit_printint_stmt;
    visitor->visit_printchar_stmt = &typecheck_visit_printchar_stmt;
    visitor->visit_printbool_stmt = &typecheck_visit_printbool_stmt;
    visitor->visit_printline_stmt = &typecheck_visit_pass;
    visitor->visit_assignment_stmt = &typecheck_visit_assignment_stmt;
    visitor->visit_if_stmt = &typecheck_visit_if_stmt;
    visitor->visit_while_stmt = &typecheck_visit_while_stmt;
    visitor->visit_for_stmt = &typecheck_visit_for_stmt;
    visitor->visit_rel_expr = &typecheck_visit_binary_expr;
    visitor->visit_add_expr = &typecheck_visit_binary_expr;
    visitor->visit_mul_expr = &typecheck_visit_binary_expr;
    visitor->visit_notfactor = &typecheck_visit_notfactor;
    visitor->visit_call = &typecheck_visit_call;
    visitor->visit_callparam_list = &typecheck_visit_callparam_list;
    visitor->visit_identifier = &typecheck_visit_identifier;
    visitor->visit_literal = &typecheck_visit_pass;
    visitor->visit_add_op = &typecheck_visit_pass;
    visitor->visit_mul_op = &typecheck_visit_pass;
    visitor->visit_rel_op = &typecheck_visit_pass;
    visitor->visit_not_op = &typecheck_visit_pass;

    return visitor;
}

void
typecheck_visit_pass(struct _Visitor *visitor, struct AstNode *node)
{
    ast_node_accept_children(node->children, visitor);
}

void
typecheck_visit_program(struct _Visitor *visitor, struct AstNode *node)
{
    node->symbol = symbol_new(NULL);
    global_symtab = node->symbol;
    symtab = global_symtab;
    ast_node_accept_children(node->children, visitor);
}

void
typecheck_visit_programdecl(struct _Visitor *visitor, struct AstNode *node)
{
    is_vardecl = TRUE;
    ast_node_accept_children(node->children, visitor);
}

void
typecheck_visit_procfunc_list(struct _Visitor *visitor, struct AstNode *node)
{
    symtab = global_symtab;
    ast_node_accept_children(node->children, visitor);
}

void
typecheck_visit_procfunc(struct _Visitor *visitor, struct AstNode *node)
{
    is_vardecl = FALSE;
    symtab = node->symbol;
    procfunc_symbol = node->children->symbol;
    ast_node_accept_children(node->children, visitor);
}

void
typecheck_visit_vardecl_list (struct _Visitor *visitor, struct AstNode *node)
{
    is_vardecl = TRUE;
    symtab = node->parent->symbol;
    ast_node_accept_children(node->children, visitor);
}

void
typecheck_visit_vardecl (struct _Visitor *visitor, struct AstNode *node)
{
    declared_type = node->type;
    ast_node_accept_children(node->children, visitor);
}

void
typecheck_visit_parameter (struct _Visitor *visitor, struct AstNode *node)
{
    is_vardecl = TRUE;
    declared_type = node->type;
    ast_node_accept_children(node->children, visitor);
}

void
typecheck_visit_statement_list(struct _Visitor *visitor, struct AstNode *node)
{
    is_vardecl = FALSE;
    declared_type = VOID;
    symtab = node->parent->symbol;
    ast_node_accept_children(node->children, visitor);
}

static void
_typecheck_print_stmt(struct AstNode *node, Type type, const char *type_str)
{
    if (_get_expression_type(node->children) != type) {
        node->type = ERROR;
        fprintf(stderr,
                "Error: Expression Print%s statement must be of "
                "%s type. Check line %d.\n",
                type_str, type_get_lexeme(type), node->linenum);
    }
}

void
typecheck_visit_printint_stmt (struct _Visitor *visitor, struct AstNode *node)
{
    _typecheck_print_stmt(node, INTEGER, "Int");
    ast_node_accept_children(node->children, visitor);
}

void
typecheck_visit_printchar_stmt (struct _Visitor *visitor, struct AstNode *node)
{
    _typecheck_print_stmt(node, CHAR, "Char");
    ast_node_accept_children(node->children, visitor);
}

void
typecheck_visit_printbool_stmt (struct _Visitor *visitor, struct AstNode *node)
{
    _typecheck_print_stmt(node, BOOLEAN, "Bool");
    ast_node_accept_children(node->children, visitor);
}

void
typecheck_visit_assignment_stmt (struct _Visitor *visitor, struct AstNode *node)
{
    struct AstNode *node1 = node->children;
    struct AstNode *node2 = node1->sibling;

    if (node1->kind != IDENTIFIER) {
        fprintf(stderr,
                "Error: Left side of assignment must be an Identifier. Check line %d.\n",
                node->linenum);
    } else if (_get_expression_type(node1) != _get_expression_type(node2)) {
        node->type = ERROR;
        fprintf(stderr,
                "Error: Incompatible types on assignment operation in line %d.\n",
                node->linenum);
    }
    ast_node_accept_children(node->children, visitor);
}

void
typecheck_visit_if_stmt (struct _Visitor *visitor, struct AstNode *node)
{
    struct AstNode *expr = node->children;

    if (_get_expression_type(expr) != BOOLEAN) {
        node->type = ERROR;
        fprintf(stderr,
                "Error: Condition for if statement must be of Boolean type. "
                "Check line %d.\n", node->linenum);
    }
    ast_node_accept_children(node->children, visitor);
}

void
typecheck_visit_while_stmt (struct _Visitor *visitor, struct AstNode *node)
{
    Type type;
    struct AstNode *expr = node->children;

    type = _get_expression_type(expr);
    if (type != BOOLEAN) {
        node->type = ERROR;
        fprintf(stderr,
                "Error: Expression in While statement must be of "
                "Boolean type. Check line %d.\n", expr->linenum);
    }
    ast_node_accept_children(node->children, visitor);
}

void
typecheck_visit_for_stmt (struct _Visitor *visitor, struct AstNode *node)
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
    ast_node_accept_children(node->children, visitor);
}

void
typecheck_visit_binary_expr (struct _Visitor *visitor, struct AstNode *node)
{
    Type type1;
    Type type2;
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
    ast_node_accept_children(node->children, visitor);
}

void
typecheck_visit_notfactor (struct _Visitor *visitor, struct AstNode *node)
{
    Type type = _get_expression_type(node->children->sibling);

    if (type != BOOLEAN) {
        node->type = ERROR;
        fprintf(stderr,
                "Error: Operation 'not' over non-boolean type on line %d.\n",
                node->linenum);
    }
    ast_node_accept_children(node->children, visitor);
}

void
typecheck_visit_call (struct _Visitor *visitor, struct AstNode *node)
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
    ast_node_accept_children(node->children, visitor);
}

void
typecheck_visit_callparam_list (struct _Visitor *visitor, struct AstNode *node)
{
    int i;
    struct AstNode *temp;

    i = 0;
    for (temp = node->children; temp != NULL; temp = temp->sibling) {
        if (temp->type != procfunc_symbol->param_types[i]) {
            fprintf(stderr, "Error: Call '%s' on line %d, expecting %s "
                    "on parameter %d (",
                    procfunc_symbol->name, node->linenum,
                    type_get_lexeme(procfunc_symbol->param_types[i]), i+1);
            if (temp->symbol != NULL)
                fprintf(stderr, "'%s'", temp->symbol->name);
            else
                value_print(stderr, &temp->value, temp->type);

            fprintf(stderr, "), received %s.\n",
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

    ast_node_accept_children(node->children, visitor);
}

void
typecheck_visit_identifier (struct _Visitor *visitor, struct AstNode *node)
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
            /*int i;
            //procfunc_symbol->param_types[procfunc_symbol->params] = node->symbol->type;
            //procfunc_symbol->params++;
            fprintf(stderr, "Parameter: %s (%x)\n",
                    procfunc_symbol->name, procfunc_symbol);
            fprintf(stderr, "types (%d): ", procfunc_symbol->params);
            for (i = 0; i < procfunc_symbol->params; i++) {
                fprintf(stderr, "%d ",
                        procfunc_symbol->param_types[procfunc_symbol->params]);
            }
            fprintf(stderr, "\n");*/
        }
    }

    ast_node_accept_children(node->children, visitor);
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

        case CALL:
            name = node->children->symbol->name;
            sym = symbol_lookup(global_symtab, name);
            if (sym != NULL) {
               node->type = sym->type;
               return sym->type;
            }
            break;

        default:
            return node->type;
    }
}
