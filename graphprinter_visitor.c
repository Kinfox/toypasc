#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "graphprinter_visitor.h"

static bool is_symboldecl = FALSE;
static void _print_symbol_table(struct AstNode *node);
static void _print_symbols(Symbol *symbol);

Visitor *
graphprinter_new()
{
    Visitor *gp_visitor = (Visitor *) malloc (sizeof(Visitor));

    gp_visitor->visit_program = &graphprinter_visit_program;
    gp_visitor->visit_programdecl = &graphprinter_visit_programdecl;
    gp_visitor->visit_vardecl_list = &graphprinter_visit_vardecl_list;
    gp_visitor->visit_vardecl = &graphprinter_visit_simplenode;
    gp_visitor->visit_identifier_list = &graphprinter_visit_identifier_list;
    gp_visitor->visit_procfunc_list = &graphprinter_visit_procfunc_list;
    gp_visitor->visit_procedure = &graphprinter_visit_procfunc;
    gp_visitor->visit_function = &graphprinter_visit_procfunc;
    gp_visitor->visit_param_list = &graphprinter_visit_param_list;
    gp_visitor->visit_parameter = &graphprinter_visit_simplenode;
    gp_visitor->visit_statement_list = &graphprinter_visit_statement_list;
    gp_visitor->visit_printint_stmt = &graphprinter_visit_simplenode;
    gp_visitor->visit_printchar_stmt = &graphprinter_visit_simplenode;
    gp_visitor->visit_printbool_stmt = &graphprinter_visit_simplenode;
    gp_visitor->visit_printline_stmt = &graphprinter_visit_simplenode;
    gp_visitor->visit_assignment_stmt = &graphprinter_visit_simplenode;
    gp_visitor->visit_if_stmt = &graphprinter_visit_simplenode;
    gp_visitor->visit_while_stmt = &graphprinter_visit_simplenode;
    gp_visitor->visit_for_stmt = &graphprinter_visit_simplenode;
    gp_visitor->visit_rel_expr = &graphprinter_visit_binary_expr;
    gp_visitor->visit_add_expr = &graphprinter_visit_binary_expr;
    gp_visitor->visit_mul_expr = &graphprinter_visit_binary_expr;
    gp_visitor->visit_notfactor = &graphprinter_visit_simplenode;
    gp_visitor->visit_call = &graphprinter_visit_simplenode;
    gp_visitor->visit_callparam_list = &graphprinter_visit_callparam_list;
    gp_visitor->visit_identifier = &graphprinter_visit_identifier;
    gp_visitor->visit_literal = &graphprinter_visit_literal;
    gp_visitor->close_group = &graphprinter_close_group;
}

void
graphprinter_visit_program(struct AstNode *node)
{
    struct AstNode *temp;

    printf("/* toypasc AST graph. */\n");
    printf("digraph {\n");

    printf("\tremincross=true;\n");
    printf("\tordering=out;\n");
    printf("\tcompound=true;\n");
    printf("\tranksep=1.0;\n");
    printf("\tnode [fontsize=11,fontname=Courier];\n");
    printf("\tedge [color=\"#22DDAA\"];\n\n");

    printf("\tnode_%x [label=\"%s\",style=", node, node->name);
    printf("filled,color=orange,fillcolor=\"#FFEEEE\"];\n");

    _print_symbol_table(node);
}

void
graphprinter_visit_simplenode (struct AstNode *node)
{
    printf("\tnode_%x -> node_%x;\n", node->parent, node);
    printf("\tnode_%x [label=\"%s\",style=", node, node->name);
    printf("filled,fillcolor=\"#EEFFEE\",color=\"#%s\"];\n",
           (node->type == ERROR) ? "FF0000" : "EEFFEE");
}

void
graphprinter_visit_programdecl(struct AstNode *node)
{
    is_symboldecl = TRUE;
    printf("\tnode_%x -> node_%x;\n", node->parent, node);
    printf("\tnode_%x [label=\"%s\",style=", node, node->name);
    printf("filled,color=\"#22DDAA\",fillcolor=\"#EEFFEE\"];\n");
}

void
graphprinter_visit_vardecl_list (struct AstNode *node)
{
    is_symboldecl = TRUE;
    printf("\tnode_%x -> node_%x;\n", node->parent, node);
    printf("\tnode_%x [label=\"%s\",style=", node, node->name);
    printf("filled,color=\"#22DDAA\",fillcolor=\"#EEFFEE\"];\n");
    printf("subgraph cluster_%x {\n\tstyle=dotted;\n", node);
}

void
graphprinter_visit_identifier_list (struct AstNode *node)
{
    printf("\tnode_%x -> node_%x;\n", node->parent, node);
    printf("\tnode_%x [label=\"%s\",style=", node, node->name);
    printf("filled,color=\"#22DDAA\",fillcolor=\"#EEFFEE\"];\n");
    printf("subgraph cluster_%x {\n\tstyle=dotted;\n", node);
}

void
graphprinter_visit_procfunc_list (struct AstNode *node)
{
    printf("\tnode_%x -> node_%x;\n", node->parent, node);
    printf("\tnode_%x [label=\"%s\",style=", node, node->name);
    printf("filled,color=\"#22DDAA\",fillcolor=\"#EEFFEE\"];\n");
}

void
graphprinter_visit_procfunc (struct AstNode *node)
{
    is_symboldecl = TRUE;
    printf("\tnode_%x -> node_%x;\n", node->parent, node);
    printf("\tnode_%x [label=\"%s\\n'%s'\",style=",
           node, node->name, node->children->symbol->name);
    printf("filled,color=blue,fillcolor=\"#EEEEFF\"];\n");
    printf("subgraph cluster_%x {\n\tstyle=dotted;\n", node);
    _print_symbol_table(node);
}

void
graphprinter_visit_param_list (struct AstNode *node)
{
    is_symboldecl = TRUE;
    printf("\tnode_%x -> node_%x;\n", node->parent, node);
    printf("\tnode_%x [label=\"%s\",style=", node, node->name);
    printf("filled,color=\"#22DDAA\",fillcolor=\"#EEFFEE\"];\n");
    printf("subgraph cluster_%x {\n\tstyle=dotted;\n", node);
}

void
graphprinter_visit_statement_list (struct AstNode *node)
{
    is_symboldecl = FALSE;
    printf("\tnode_%x -> node_%x;\n", node->parent, node);
    printf("\tnode_%x [label=\"%s\",style=", node, node->name);
    printf("filled,color=\"#22DDAA\",fillcolor=\"#EEFFEE\"];\n");
    printf("subgraph cluster_%x {\n\tstyle=dotted;\n", node);
}

void
graphprinter_visit_binary_expr (struct AstNode *node)
{
    printf("\tnode_%x -> node_%x;\n", node->parent, node);
    printf("\tnode_%x [label=\"%s\\n(%s)\",style=",
           node, node->name, node->children->sibling->name);
    printf("filled,fillcolor=\"#EEFFEE\",color=\"#%s\"];\n",
           (node->type == ERROR) ? "FF0000" : "EEFFEE");
}

void
graphprinter_visit_callparam_list (struct AstNode *node)
{
    printf("\tnode_%x -> node_%x;\n", node->parent, node);
    printf("\tnode_%x [label=\"%s\",style=", node, node->name);
    printf("filled,color=\"#22DDAA\",fillcolor=\"#EEFFEE\"];\n");
    printf("subgraph cluster_%x {\n\tstyle=dotted;\n", node);
}

void
graphprinter_visit_identifier (struct AstNode *node)
{
    printf("\tnode_%x -> node_%x;\n", node->parent, node);
    printf("\tnode_%x [label=\"%s\",style=", node, node->name);
    printf("filled,color=\"#EEFFEE\"];\n");

    if (node->symbol->decl_linenum == 0) {
        printf("\tsymbol_%x [label=\"'%s'\\nundeclared\",",
               node->symbol, node->symbol->name);
        printf("color=red,fillcolor=\"#FFEEEE\",style=filled];\n");
    }

    printf("\tnode_%x -> symbol_%x", node, node->symbol);

    if (is_symboldecl)
        printf(" [color=\"#00FF00\"]");

    printf(";\n");
}

void
graphprinter_visit_literal (struct AstNode *node)
{
    printf("\tnode_%x -> literal_%x;\n", node->parent, node);
    printf("\tliteral_%x [label=\"", node);
    value_print(&node->value, node->type);
    printf("\",style=filled,color=\"#FFFFCC\"];\n");
}

void
graphprinter_close_group ()
{
    printf("}\n");
}

static void
_print_graph(struct AstNode *self)
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
            printf("filled,color=orange,fillcolor=\"#FFEEEE\"];\n");
        else if (is_funcproc)
            printf("filled,color=blue,fillcolor=\"#EEEEFF\"];\n");
        else
            printf("filled,color=\"#22DDAA\",fillcolor=\"#EEFFEE\"];\n");
        printf("subgraph cluster_%x {\n\tstyle=dotted;\n", self, self->name);
    } else
        printf("filled,color=\"#EEFFEE\"];\n");

    if (is_funcproc) {
        printf("\tsubgraph cluster_symtab_0x%x {\tstyle=filled;\n", self->symbol);
        printf("\tcolor=\"#EFEFEF\";\n\tfontname=Courier;\n");
        printf("\tnode [style=filled,color=white,fillcolor=\"#CCFF99\"];\n");
        //_ast_node_print_graph_symbol_table(self->symbol);
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

    _print_graph(self->children);

    if (is_cluster)
        printf("}\n");

    _print_graph(self->sibling);
}

static void
_print_symbol_table(struct AstNode *node)
{
    if (node->symbol->next == NULL)
        return;

    printf("\tnode_%x -> symbol_%x [lhead=cluster_symtab_%x,color=",
           node, node->symbol->next, node);
    if (node->parent == NULL)
        printf("orange];\n");
    else
        printf("blue];\n");

    printf("\tsubgraph cluster_symtab_%x {\n\tstyle=filled;\n", node);

    if (node->parent == NULL)
        printf("\tcolor=orange;\n");
    else
        printf("\tcolor=blue;\n");

    printf("\tstyle=filled;\n\tfillcolor=\"#EFEFEF\";\n\tfontname=Courier;\n");
    printf("\tnode [style=filled,color=white,fillcolor=\"#CCFF99\"];\n");

    _print_symbols(node->symbol->next);

    printf("\t}\n");
}

static void
_print_symbols(Symbol *symbol)
{
    if (symbol == NULL)
        return;

    if (symbol->name != NULL) {
        printf("\t\tsymbol_%x [shape=record,label=\"{", symbol);
        printf("Symbol|Address: 0x%x\\l|lexeme: %s\\l|", symbol, symbol->name);
        printf("type: %s\\l}\"", type_get_lexeme(symbol->type));
        printf(",style=filled,color=white,fillcolor=\"#CCFF99\"];\n");
    }

    _print_symbols(symbol->next);
}

