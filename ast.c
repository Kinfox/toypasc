#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "typecheck_visitor.h"

static void _ast_node_print_graph(struct AstNode *self);
static void _ast_node_print_graph_symbol_table(Symbol *symbol);

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
ast_node_add_child(struct AstNode *self, struct AstNode *child)
{
    if (self == NULL || child == NULL)
        return;

    if (self->children == NULL)
        self->children = child;
    else
        ast_node_add_sibling(self->children, child);
}

void
ast_node_add_sibling(struct AstNode *self, struct AstNode *sibling)
{
    struct AstNode *temp;

    if (self == NULL || sibling == NULL)
        return;

    if (self->sibling == NULL) {
        self->sibling = sibling;
    } else {
        temp = self->sibling;
        while (temp->sibling != NULL)
            temp = temp->sibling;
        temp->sibling = sibling;
    }
}

void
ast_node_accept(struct AstNode *self, Visitor *visitor)
{
    switch (self->kind) {
        case PROGRAM:
            visitor->visit_program(self);
            break;
        case PROGDECL:
            visitor->visit_programdecl(self);
            break;
        case VARDECL_LIST:
            visitor->visit_vardecl_list(self);
            break;
        case VARDECL:
            visitor->visit_vardecl(self);
            break;
        case IDENT_LIST:
            visitor->visit_identifier_list(self);
            break;
        case PROCFUNC_LIST:
            visitor->visit_procfunc_list(self);
            break;
        case PROCEDURE:
            visitor->visit_procedure(self);
            break;
        case FUNCTION:
            visitor->visit_function(self);
            break;
        case PARAM_LIST:
            visitor->visit_param_list(self);
            break;
        case PARAMETER:
            visitor->visit_parameter(self);
            break;
        case STATEMENT_LIST:
            visitor->visit_statement_list(self);
            break;
        case PRINTINT_STMT:
            visitor->visit_printint_stmt(self);
            break;
        case PRINTCHAR_STMT:
            visitor->visit_printchar_stmt(self);
            break;
        case PRINTBOOL_STMT:
            visitor->visit_printbool_stmt(self);
            break;
        case PRINTLINE_STMT:
            visitor->visit_printline_stmt(self);
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
}

void
ast_node_fill_symbol_table(struct AstNode *self, Symbol *symtab)
{
    Symbol *symbol = NULL;
    struct AstNode *temp;

    if (self == NULL)
        return;

    if (self->kind == IDENTIFIER) {
        if (symtab != global_symbol_table) {
            symbol = symbol_lookup(global_symbol_table, self->symbol->name);
            if (symbol == NULL)
                self->symbol = symbol_insert(symtab, self->symbol);
            else
                self->symbol = symbol;
        } else
            self->symbol = symbol_insert(symtab, self->symbol);
    } else if (self->kind == FUNCTION || self->kind == PROCEDURE)
        // Primeiro filho de uma funcao ou procedimento eh um Identifier.
        self->children->symbol =
                symbol_insert(symtab, self->children->symbol);
    else
        for (temp = self->children; temp != NULL; temp = temp->sibling)
            ast_node_fill_symbol_table(temp, symtab);
}

void
ast_node_set_type(struct AstNode *self, Type type)
{
    struct AstNode *temp;

    if (self == NULL)
        return;

    self->type = type;
    if (self->symbol != NULL)
        self->symbol->type = type;

    if (self->kind == IDENT_LIST) {
        for (temp = self->children; temp != NULL; temp = temp->sibling)
            ast_node_set_type(temp, type);
    } else
        ast_node_set_type(self->children, type);
}

Type
ast_node_get_type(struct AstNode *self)
{
    if (self->type == NONE_TYPE)
        return ast_node_get_type(self->children);
    else
        return self->type;
}

void
ast_node_print(struct AstNode *self)
{
    int i;
    struct AstNode *temp;

    if (self == NULL)
        return;

    printf("(AstNode) %x : %s\n", self, self->name);
    printf("kind: %d\n", self->kind);
    printf("type: %d\n", self->type);
    printf("value: ");
    value_print(&self->value, self->type);
    printf("\nlinenum: %d\n", self->linenum);
    if (self->symbol != NULL)
        printf("symbol: %x (\"%s\")\n", self->symbol, self->symbol->name);

    if (self->children != NULL) {
        printf("Children\n");
        temp = self->children;
        while (temp != NULL) {
            printf("\t(AstNode) %x", temp);
            if (temp->name != NULL)
                printf(" : %s", temp->name);
            printf("\n");
            temp = temp->sibling;
        }
    }
    printf("\n");

    ast_node_print(self->children);
    ast_node_print(self->sibling);
}

void
ast_node_print_graph(struct AstNode *self)
{
    printf("/* toypasc AST graph. */\n");
    printf("digraph {\n");
    printf("ranksep=1.0;\n");
    printf("\tnode [fontsize=11,fontname=Courier];\n");
    printf("\tedge [color=\"#22DDAA\"];\n");

    printf("\tsubgraph cluster_symtab_global {\tstyle=filled;\n");
    printf("\tcolor=\"#EFEFEF\";\n\tfontname=Courier;\n");
    printf("\tnode [style=filled,color=white,fillcolor=\"#CCFF99\"];\n");
    _ast_node_print_graph_symbol_table(self->symbol);
    printf("\t}\n");

    _ast_node_print_graph(self);

    printf("}\n");
}

static void
_ast_node_print_graph(struct AstNode *self)
{
    int i;
    bool is_program;
    bool is_cluster;
    bool is_funcproc;
    struct AstNode *temp;

    if (self == NULL)
        return;

    is_program = self->kind == PROGRAM;
    is_funcproc = self->kind == PROCEDURE || self->kind == FUNCTION;
    is_cluster = is_program || is_funcproc ||
                 self->kind == PROGDECL ||
                 strstr(self->name, "List");

    printf("\tnode_%x [label=\"%s\",style=", self, self->name);

    if (is_cluster) {
        if (is_program)
            printf("filled,color=\"#FF0000\",fillcolor=\"#FFEEEE\"];\n");
        else if (is_funcproc)
            printf("filled,color=\"#0000FF\",fillcolor=\"#EEEEFF\"];\n");
        else
            printf("filled,color=\"#22DDAA\",fillcolor=\"#EEFFEE\"];\n");
        printf("subgraph cluster_%x {\n\tstyle=dotted;\n", self, self->name);
    } else
        printf("filled,color=\"#EEFFEE\"];\n");

    if (is_funcproc) {
        printf("\tsubgraph cluster_symtab_0x%x {\tstyle=filled;\n", self->symbol);
        printf("\tcolor=\"#EFEFEF\";\n\tfontname=Courier;\n");
        printf("\tnode [style=filled,color=white,fillcolor=\"#CCFF99\"];\n");
        _ast_node_print_graph_symbol_table(self->symbol);
        printf("\t}\n");
    }

    if (self->children != NULL) {
        temp = self->children;
        while (temp != NULL) {
            printf("\tnode_%x -> node_%x;\n", self, temp);
            temp = temp->sibling;
        }
    } else {
        if (self->symbol != NULL) {
            printf("\tnode_%x -> symbol_%x [color=lightgray,headport=n];\n",
                   self, self->symbol);
        } else if (strstr(self->name, "Literal")) {
            printf("\tliteral_%x [label=\"", self);
            value_print(&self->value, self->type);
            printf("\",style=filled,color=\"#FFFFCC\"];\n");
            printf("\tnode_%x -> literal_%x;\n", self, self);
        }

    }

    _ast_node_print_graph(self->children);

    if (is_cluster)
        printf("}\n");

    _ast_node_print_graph(self->sibling);
}

static void
_ast_node_print_graph_symbol_table(Symbol *symbol)
{
    if (symbol == NULL)
        return;

    if (symbol->name != NULL) {
        printf("\t\tsymbol_%x [shape=record,label=\"{", symbol);
        printf("Symbol|Address: 0x%x\\l|lexeme: %s\\l|", symbol, symbol->name);
        printf("type: %s\\l}\"", type_get_lexeme(symbol->type));
        printf(",style=filled,color=white,fillcolor=\"#CCFF99\"];\n");
    }

    _ast_node_print_graph_symbol_table(symbol->next);
}

