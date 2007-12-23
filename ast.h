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
    void (*visit_printint_stmt)(struct AstNode *);
    void (*visit_printchar_stmt)(struct AstNode *);
    void (*visit_printbool_stmt)(struct AstNode *);
    void (*visit_printline_stmt)(struct AstNode *);
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
} Visitor;

struct AstNode *ast_node_new(const char* name, int kind, int type,
                             int linenum, Symbol *symbol);
void ast_node_destroy(struct AstNode *self);

void ast_node_add_child(struct AstNode *self, struct AstNode *child);
void ast_node_add_sibling(struct AstNode *self, struct AstNode *sibling);

void ast_node_accept(struct AstNode *self, Visitor *visitor);

Type ast_node_get_type(struct AstNode *self);
void ast_node_set_type(struct AstNode *self, Type type);

void ast_node_fill_symbol_table(struct AstNode *self, Symbol *symtab);

void ast_node_print(struct AstNode *self);
void ast_node_print_graph(struct AstNode *self);

#endif // AST_H
