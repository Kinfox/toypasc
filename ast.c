#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

//static void _ast_node_print_graph(struct AstNode *node);
//static int graph_code;

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

/* Draft...
voi
ast_node_print_graph(struct AstNode *node)
{
    graph_code = 0;
    printf("/* toypasc AST graph. *\n");
    printf("digraph {\n");
    _ast_node_print_graph(node);
    printf("}\n");
}

static void
_ast_node_print_graph(struct AstNode *node)
{
    int i;
    int code;

    if (node == NULL)
        return;

    code = graph_code++;

    printf("\t%s_%d [label=%s]\n", node->name, code, node->name);
    for (i = 0; i < AST_CHILDREN_NUM; i++) {
        if (node->children[i] != NULL)
            printf("\t%s_%d -> %s_%d\n", node->name, code,
                                         node->children[i]->name,
                                         graph_code + 1);
        _ast_node_print_graph(node->children[i]);
    }

    if (node->next != NULL)
        printf("\t%s_%d -> %s_%d\n", node->next->name, code,
                                     node->children[i]->name,
                                     graph_code + 1);
    _ast_node_print_graph(node->next);
}
*/
