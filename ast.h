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
    struct AstNode* parent;
    struct AstNode* children;
    struct AstNode* sibling;
};

typedef struct {
    void (*visit_program)(struct AstNode *);
    void (*visit_programdecl)(struct AstNode *);
    void (*visit_vardecl_list)(struct AstNode *);
    void (*visit_vardecl)(struct AstNode *);
    void (*visit_identifier_list)(struct AstNode *);
    void (*visit_identifier)(struct AstNode *);
    void (*visit_procfunc_list)(struct AstNode *);
    void (*visit_procedure)(struct AstNode *);
    void (*visit_function)(struct AstNode *);
    void (*visit_param_list)(struct AstNode *);
    void (*visit_parameter)(struct AstNode *);
    void (*visit_statement_list)(struct AstNode *);
    void (*visit_print_stmt)(struct AstNode *);
    void (*visit_assignment_stmt)(struct AstNode *);
    void (*visit_if_stmt)(struct AstNode *);
    void (*visit_while_stmt)(struct AstNode *);
    void (*visit_for_stmt)(struct AstNode *);
    void (*visit_rel_expr)(struct AstNode *);
    void (*visit_add_expr)(struct AstNode *);
    void (*visit_mul_expr)(struct AstNode *);
    void (*visit_notfactor)(struct AstNode *);
    void (*visit_call)(struct AstNode *);
    void (*visit_callparam_list)(struct AstNode *);
    void (*visit_literal)(struct AstNode *);
    void (*close_group)();
} Visitor;

struct AstNode *ast_node_new(const char* name, int kind, int type,
                             int linenum, Symbol *symbol);
void ast_node_destroy(struct AstNode *self);

void ast_node_add_child(struct AstNode *self, struct AstNode *child);
void ast_node_add_sibling(struct AstNode *self, struct AstNode *sibling);
void ast_node_accept(struct AstNode *self, Visitor *visitor);

#endif // AST_H
