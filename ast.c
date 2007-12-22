#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

static void _ast_node_print_graph(struct AstNode *node);
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
ast_node_destroy(struct AstNode *node)
{
    if (node != NULL) {
        ast_node_destroy(node->children);
        ast_node_destroy(node->sibling);
        free(node);
    }
}

void
ast_node_add_child(struct AstNode *node, struct AstNode *child)
{
    struct AstNode *temp;

    if (node == NULL || child == NULL)
        return;

    if (node->children == NULL)
        node->children = child;
    else
        ast_node_add_sibling(node->children, child);
}

void
ast_node_add_sibling(struct AstNode *node, struct AstNode *sibling)
{
    struct AstNode *temp;

    if (node == NULL || sibling == NULL)
        return;

    if (node->sibling == NULL) {
        node->sibling = sibling;
    } else {
        temp = node->sibling;
        while (temp->sibling != NULL)
            temp = temp->sibling;
        temp->sibling = sibling;
    }
}

void
ast_node_fill_symbol_table(struct AstNode *node, Symbol *symtab)
{
    Symbol *symbol = NULL;
    struct AstNode *temp;

    if (node == NULL)
        return;

    if (node->kind == IDENTIFIER) {
        if (symtab != global_symbol_table) {
            symbol = symbol_lookup(global_symbol_table, node->symbol->name);
            if (symbol == NULL)
                node->symbol = symbol_insert(symtab, node->symbol);
            else
                node->symbol = symbol;
        } else
            node->symbol = symbol_insert(symtab, node->symbol);
    } else if (node->kind == FUNCTION || node->kind == PROCEDURE)
        // Primeiro filho de uma funcao ou procedimento eh um Identifier.
        node->children->symbol =
                symbol_insert(symtab, node->children->symbol);
    else
        for (temp = node->children; temp != NULL; temp = temp->sibling)
            ast_node_fill_symbol_table(temp, symtab);
}

void
ast_node_set_type(struct AstNode *node, Type type)
{
    struct AstNode *temp;

    if (node == NULL)
        return;

    node->type = type;
    if (node->symbol != NULL)
        node->symbol->type = type;

    if (node->kind == IDENT_LIST) {
        for (temp = node->children; temp != NULL; temp = temp->sibling)
            ast_node_set_type(temp, type);
    } else
        ast_node_set_type(node->children, type);
}

Type
ast_node_get_type(struct AstNode *node)
{
    if (node->type == NONE_TYPE)
        return ast_node_get_type(node->children);
    else
        return node->type;
}

void
ast_node_print(struct AstNode *node)
{
    int i;
    struct AstNode *temp;

    if (node == NULL)
        return;

    printf("(AstNode) %x : %s\n", node, node->name);
    printf("kind: %d\n", node->kind);
    printf("type: %d\n", node->type);
    printf("value: ");
    value_print(&node->value, node->type);
    printf("\nlinenum: %d\n", node->linenum);
    if (node->symbol != NULL)
        printf("symbol: %x (\"%s\")\n", node->symbol, node->symbol->name);

    if (node->children != NULL) {
        printf("Children\n");
        temp = node->children;
        while (temp != NULL) {
            printf("\t(AstNode) %x", temp);
            if (temp->name != NULL)
                printf(" : %s", temp->name);
            printf("\n");
            temp = temp->sibling;
        }
    }
    printf("\n");

    ast_node_print(node->children);
    ast_node_print(node->sibling);
}

void
ast_node_print_graph(struct AstNode *node)
{
    printf("/* toypasc AST graph. */\n");
    printf("digraph {\n");
    printf("ranksep=1.0;\n");
    printf("\tnode [fontsize=11,fontname=Courier];\n");
    printf("\tedge [color=\"#22DDAA\"];\n");

    printf("\tsubgraph cluster_symtab_global {\tstyle=filled;\n");
    printf("\tcolor=\"#EFEFEF\";\n\tfontname=Courier;\n");
    printf("\tnode [style=filled,color=white,fillcolor=\"#CCFF99\"];\n");
    _ast_node_print_graph_symbol_table(node->symbol);
    printf("\t}\n");

    _ast_node_print_graph(node);

    printf("}\n");
}

static void
_ast_node_print_graph(struct AstNode *node)
{
    int i;
    bool is_program;
    bool is_cluster;
    bool is_funcproc;
    struct AstNode *temp;

    if (node == NULL)
        return;

    is_program = node->kind == PROGRAM;
    is_funcproc = node->kind == PROCEDURE || node->kind == FUNCTION;
    is_cluster = is_program || is_funcproc ||
                 node->kind == PROG_DECL ||
                 strstr(node->name, "List");

    printf("\tnode_%x [label=\"%s\",style=", node, node->name);

    if (is_cluster) {
        if (is_program)
            printf("filled,color=\"#FF0000\",fillcolor=\"#FFEEEE\"];\n");
        else if (is_funcproc)
            printf("filled,color=\"#0000FF\",fillcolor=\"#EEEEFF\"];\n");
        else
            printf("filled,color=\"#22DDAA\",fillcolor=\"#EEFFEE\"];\n");
        printf("subgraph cluster_%x {\n\tstyle=dotted;\n", node, node->name);
    } else
        printf("filled,color=\"#EEFFEE\"];\n");

    if (is_funcproc) {
        printf("\tsubgraph cluster_symtab_0x%x {\tstyle=filled;\n", node->symbol);
        printf("\tcolor=\"#EFEFEF\";\n\tfontname=Courier;\n");
        printf("\tnode [style=filled,color=white,fillcolor=\"#CCFF99\"];\n");
        _ast_node_print_graph_symbol_table(node->symbol);
        printf("\t}\n");
    }

    if (node->children != NULL) {
        temp = node->children;
        while (temp != NULL) {
            printf("\tnode_%x -> node_%x;\n", node, temp);
            temp = temp->sibling;
        }
    } else {
        if (node->symbol != NULL) {
            printf("\tnode_%x -> symbol_%x [color=lightgray,headport=n];\n",
                   node, node->symbol);
        } else if (strstr(node->name, "Literal")) {
            printf("\tliteral_%x [label=\"", node);
            value_print(&node->value, node->type);
            printf("\",style=filled,color=\"#FFFFCC\"];\n");
            printf("\tnode_%x -> literal_%x;\n", node, node);
        }

    }

    _ast_node_print_graph(node->children);

    if (is_cluster)
        printf("}\n");

    _ast_node_print_graph(node->sibling);
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

