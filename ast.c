#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

static void _ast_node_print_graph(struct AstNode *node);

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
    _ast_node_print_graph(node);
    printf("}\n");
}

static void
_ast_node_print_graph(struct AstNode *node)
{
    int i;
    char *is_cluster;
    struct AstNode *temp;

    if (node == NULL)
        return;

    is_cluster = strstr(node->name, "List");
    if (is_cluster != NULL)
        printf("subgraph {\nstyle=filled;\ncolor=lightgrey;\n");

    printf("\tnode_%x [label=\"%s\",style=filled,color=\"#33FFCC\"];\n",
           node, node->name);

    if (node->children != NULL) {
        temp = node->children;
        while (temp != NULL) {
            printf("\tnode_%x -> node_%x;\n", node, temp);
            temp = temp->sibling;
        }
    } else {
        if (node->symbol != NULL) {
            printf("\tsymbol_%x [label=\"%s\",style=filled,color=\"#CCFF99\"];\n",
                   node->symbol, node->symbol->name);
            printf("\tnode_%x -> symbol_%x;\n", node, node->symbol);
        } else if (strstr(node->name, "Literal")) {
            printf("\tliteral_%x [label=\"", node);
            value_print(&node->value, node->type);
            printf("\",style=filled,color=\"#FFFFCC\"];\n");
            printf("\tnode_%x -> literal_%x;\n", node, node);
        } else if (!strcmp(node->name, "SimpleType")) {
            printf("\tSimpleType_%x [label=\"", node);
            switch (node->type){
                case INTEGER:
                    printf("Integer");
                    break;
                case BOOLEAN:
                    printf("Boolean");
                    break;
                case CHAR:
                    printf("Char");
                    break; 
            }
            printf("\",style=filled,color=\"0.578 0.289 1.000\"];\n");
            printf("\tnode_%x -> SimpleType_%x;\n", node, node);
        }

    }
    _ast_node_print_graph(node->children);
    _ast_node_print_graph(node->sibling);

    if (is_cluster != NULL)
        printf("}\n");
}
