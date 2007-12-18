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

    for (i = 0; i < 3; i++)
        node->children[i] = NULL;
    node->next = NULL;

    return node;
}

void
ast_node_destroy(struct AstNode *node)
{
    int i;

    if (node == NULL)
        return;

    for (i = 0; i < 3; i++)
        ast_node_destroy(node->children[i]);
    ast_node_destroy(node->next);

    free(node);
}

void
ast_node_print(struct AstNode *node)
{
    int i;
    struct AstNode *temp;

    if (node == NULL) {
        printf("(AstNode) NULL\n");
        return;
    }

    printf("(AstNode) %x : %s\n", node, node->name);
    printf("kind: %d\n", node->kind);
    printf("type: %d\n", node->type);
    value_print(&node->value, node->type);
    printf("linenum: %d\n", node->linenum);
    printf("symbol: %x\n", node->symbol);

    printf("Sibling: %x\n", node->next);
    printf("Children\n");
    for (i = 0; i < 3; i++) {
        temp = node->children[i];
        printf("\t(AstNode) %x", temp);
        if (temp != NULL) {
            printf(" : %s", temp->name);
        }
        printf("\n");
    }

    printf("\n");

    for (i = 0; i < 3; i++) {
        if (node->children[i] != NULL)
            ast_node_print(node->children[i]);
    }
    if (node->next != NULL)
        ast_node_print(node->next);
}

/* Draft...
void
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
    for (i = 0; i < 3; i++) {
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
