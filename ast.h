#ifndef AST_H
#define AST_H

#include "base.h"
#include "parser.h"
#include "symbol_table.h"

struct AstNode {
    char *name;
    int kind;
    Type type;
    Value value;
    Symbol *symbol;
    int linenum;
    struct AstNode* children;
    struct AstNode* sibling;
};

struct AstNode *ast_node_new(const char* name, int kind, int type,
                             int linenum, Symbol *symbol);
void ast_node_destroy(struct AstNode *node);

void ast_node_add_child(struct AstNode *node, struct AstNode *child);
void ast_node_add_sibling(struct AstNode *node, struct AstNode *sibling);

Type ast_node_get_type(struct AstNode *node);
void ast_node_set_type(struct AstNode *node, Type type);

void ast_node_fill_symbol_table(struct AstNode *node, Symbol *symtab);

void ast_node_print(struct AstNode *node);
void ast_node_print_graph(struct AstNode *node, Symbol *sym_table);

#endif // AST_H
