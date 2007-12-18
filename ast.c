#include <stdlib.h>
#include "ast.h"

AstNode *
ast_node_new(int kind, int type, int linenum, Symbol *symbol)
{
    int i;
    AstNode *node;

    node = (AstNode *) malloc (sizeof(AstNode));

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
ast_node_destroy(AstNode *node)
{
    int i;

    if (node == NULL)
        return;

    for (i = 0; i < 3; i++)
        ast_node_destroy(node->children[i]);
    ast_node_destroy(node->next);

    free(node);
}

