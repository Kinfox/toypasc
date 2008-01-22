#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "typecheck_visitor.h"

struct AstNode *
ast_node_new(const char* name, int kind, int type,
             int linenum, Symbol *symbol)
{
    int i;
    struct AstNode *node;

    node = (struct AstNode *) malloc (sizeof(struct AstNode));

    if (name != NULL){
        node->name = strdup(name);
    } else
        node->name = NULL;

    node->kind = kind;
    node->type = type;
    node->linenum = linenum;
    node->symbol = symbol;
    node->visited = FALSE;
    node->parent = NULL;
    node->children = NULL;
    node->sibling = NULL;

    return node;
}

void
ast_node_destroy(struct AstNode *self)
{
    if (self != NULL) {
        ast_node_destroy(self->children);
        ast_node_destroy(self->sibling);
        free(self);
    }
}

void
ast_node_unset_visited(struct AstNode *self)
{
    if (self == NULL)
        return;

    ast_node_unset_visited(self->children);
    ast_node_unset_visited(self->sibling);
    self->visited = FALSE;
}

void
ast_node_add_child(struct AstNode *self, struct AstNode *child)
{
    struct AstNode *temp;

    if (child == NULL)
        return;

    if (self->children == NULL) {
        child->parent = self;
        self->children = child;
    } else {
        ast_node_add_sibling(self->children, child);
    }
    for (temp = child; temp != NULL; temp = temp->sibling)
        temp->parent = self;
}

void
ast_node_add_sibling(struct AstNode *self, struct AstNode *sibling)
{
    struct AstNode *temp;

    if (sibling == NULL)
        return;

    if (self->sibling == NULL) {
        self->sibling = sibling;
    } else {
        for (temp = self->sibling; temp->sibling != NULL; temp = temp->sibling)
            ;
        temp->sibling = sibling;
    }
}

void
ast_node_accept(struct AstNode *self, Visitor *visitor)
{
    struct AstNode *temp;
    bool opened_group = FALSE;

    switch (self->kind) {
        case PROGRAM:
            ast_node_unset_visited(self);
            visitor->visit_program(self);
            opened_group = TRUE;
            break;
        case PROGDECL:
            visitor->visit_programdecl(self);
            break;
        case VARDECL_LIST:
            visitor->visit_vardecl_list(self);
            opened_group = TRUE;
            break;
        case VARDECL:
            visitor->visit_vardecl(self);
            break;
        case IDENT_LIST:
            visitor->visit_identifier_list(self);
            opened_group = TRUE;
            break;
        case PROCFUNC_LIST:
            visitor->visit_procfunc_list(self);
            break;
        case PROCEDURE:
            visitor->visit_procedure(self);
            opened_group = TRUE;
            break;
        case FUNCTION:
            visitor->visit_function(self);
            opened_group = TRUE;
            break;
        case PARAM_LIST:
            visitor->visit_param_list(self);
            opened_group = TRUE;
            break;
        case PARAMETER:
            visitor->visit_parameter(self);
            break;
        case STATEMENT_LIST:
            visitor->visit_statement_list(self);
            opened_group = TRUE;
            break;
        case PRINTINT_STMT:
        case PRINTCHAR_STMT:
        case PRINTBOOL_STMT:
        case PRINTLINE_STMT:
            visitor->visit_print_stmt(self);
            break;
        case ASSIGNMENT_STMT:
            visitor->visit_assignment_stmt(self);
            break;
        case IF_STMT:
            visitor->visit_if_stmt(self);
            break;
        case WHILE_STMT:
            visitor->visit_while_stmt(self);
            break;
        case FOR_STMT:
            visitor->visit_for_stmt(self);
            break;
        case REL_EXPR:
            visitor->visit_rel_expr(self);
            break;
        case ADD_EXPR:
            visitor->visit_add_expr(self);
            break;
        case MUL_EXPR:
            visitor->visit_mul_expr(self);
            break;
        case NOTFACTOR:
            visitor->visit_notfactor(self);
            break;
        case CALL:
            visitor->visit_call(self);
            break;
        case CALLPARAM_LIST:
            visitor->visit_callparam_list(self);
            opened_group = TRUE;
            break;
        case IDENTIFIER:
            visitor->visit_identifier(self);
            break;
        case INT_LITERAL:
        case BOOL_LITERAL:
        case CHAR_LITERAL:
            visitor->visit_literal(self);
            break;
    }

    for (temp = self->children; temp != NULL; temp = temp->sibling)
        ast_node_accept(temp, visitor);

    if (opened_group) {
        if (visitor->close_group != NULL)
            visitor->close_group();
    }
}

