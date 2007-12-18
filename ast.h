#ifndef AST_H
#define AST_H

#include "symbol_table.h"

typedef struct _AstNode {
    int kind;
    int type;
    int linenum;
    Symbol *symbol;
    struct _AstNode* children[3];
    struct _AstNode* next;
} AstNode;

AstNode *ast_node_new(int kind, int type, int linenum, Symbol *symbol);
void ast_node_destroy(AstNode *node);

#endif // AST_H
