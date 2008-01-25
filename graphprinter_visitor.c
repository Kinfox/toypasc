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
    Visitor *visitor = (Visitor *) malloc (sizeof(Visitor));

    visitor->visit_program = &graphprinter_visit_program;
    visitor->visit_programdecl = &graphprinter_visit_programdecl;
    visitor->visit_vardecl_list = &graphprinter_visit_vardecl_list;
    visitor->visit_vardecl = &graphprinter_visit_simplenode;
    visitor->visit_identifier_list = &graphprinter_visit_identifier_list;
    visitor->visit_procfunc_list = &graphprinter_visit_procfunc_list;
    visitor->visit_procedure = &graphprinter_visit_procfunc;
    visitor->visit_function = &graphprinter_visit_procfunc;
    visitor->visit_param_list = &graphprinter_visit_param_list;
    visitor->visit_parameter = &graphprinter_visit_simplenode;
    visitor->visit_statement_list = &graphprinter_visit_statement_list;
    visitor->visit_printint_stmt = &graphprinter_visit_simplenode;
    visitor->visit_printchar_stmt = &graphprinter_visit_simplenode;
    visitor->visit_printbool_stmt = &graphprinter_visit_simplenode;
    visitor->visit_printline_stmt = &graphprinter_visit_simplenode;
    visitor->visit_assignment_stmt = &graphprinter_visit_simplenode;
    visitor->visit_if_stmt = &graphprinter_visit_simplenode;
    visitor->visit_while_stmt = &graphprinter_visit_simplenode;
    visitor->visit_for_stmt = &graphprinter_visit_simplenode;
    visitor->visit_rel_expr = &graphprinter_visit_binary_expr;
    visitor->visit_add_expr = &graphprinter_visit_binary_expr;
    visitor->visit_mul_expr = &graphprinter_visit_binary_expr;
    visitor->visit_notfactor = &graphprinter_visit_simplenode;
    visitor->visit_call = &graphprinter_visit_call;
    visitor->visit_callparam_list = &graphprinter_visit_callparam_list;
    visitor->visit_identifier = &graphprinter_visit_identifier;
    visitor->visit_literal = &graphprinter_visit_literal;
    visitor->visit_add_op = NULL;
    visitor->visit_mul_op = NULL;
    visitor->visit_rel_op = NULL;
    visitor->visit_not_op = NULL;

    return visitor;
}

void
graphprinter_visit_program(struct _Visitor *visitor, struct AstNode *node)
{
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

    ast_node_accept_children(node->children, visitor);
    printf("}\n");
}

void
graphprinter_visit_simplenode (struct _Visitor *visitor, struct AstNode *node)
{
    printf("\tnode_%x -> node_%x;\n", node->parent, node);
    printf("\tnode_%x [label=\"%s\",style=", node, node->name);
    printf("filled,fillcolor=\"#EEFFEE\",color=\"#%s\"];\n",
           (node->type == ERROR) ? "FF0000" : "EEFFEE");
    ast_node_accept_children(node->children, visitor);
}

void
graphprinter_visit_programdecl(struct _Visitor *visitor, struct AstNode *node)
{
    is_symboldecl = TRUE;
    printf("\tnode_%x -> node_%x;\n", node->parent, node);
    printf("\tnode_%x [label=\"%s\",style=", node, node->name);
    printf("filled,color=\"#22DDAA\",fillcolor=\"#EEFFEE\"];\n");
    ast_node_accept_children(node->children, visitor);
}

void
graphprinter_visit_vardecl_list (struct _Visitor *visitor, struct AstNode *node)
{
    is_symboldecl = TRUE;
    printf("\tnode_%x -> node_%x;\n", node->parent, node);
    printf("\tnode_%x [label=\"%s\",style=", node, node->name);
    printf("filled,color=\"#22DDAA\",fillcolor=\"#EEFFEE\"];\n");
    printf("\nsubgraph cluster_%x {\n\tstyle=dotted;\n", node);
    ast_node_accept_children(node->children, visitor);
    printf("}\n\n");
}

void
graphprinter_visit_identifier_list (struct _Visitor *visitor, struct AstNode *node)
{
    printf("\tnode_%x -> node_%x;\n", node->parent, node);
    printf("\tnode_%x [label=\"%s\",style=", node, node->name);
    printf("filled,color=\"#22DDAA\",fillcolor=\"#EEFFEE\"];\n");
    printf("\nsubgraph cluster_%x {\n\tstyle=dotted;\n", node);
    ast_node_accept_children(node->children, visitor);
    printf("}\n\n");
}

void
graphprinter_visit_procfunc_list (struct _Visitor *visitor, struct AstNode *node)
{
    printf("\tnode_%x -> node_%x;\n", node->parent, node);
    printf("\tnode_%x [label=\"%s\",style=", node, node->name);
    printf("filled,color=\"#22DDAA\",fillcolor=\"#EEFFEE\"];\n");
    ast_node_accept_children(node->children, visitor);
}

void
graphprinter_visit_procfunc (struct _Visitor *visitor, struct AstNode *node)
{
    is_symboldecl = TRUE;
    printf("\tnode_%x -> node_%x;\n", node->parent, node);
    printf("\tnode_%x [label=\"%s\\n'%s'\",style=",
           node, node->name, node->children->symbol->name);
    printf("filled,color=blue,fillcolor=\"#EEEEFF\"];\n");
    printf("\nsubgraph cluster_%x {\n\tstyle=dotted;\n", node);

    _print_symbol_table(node);
    ast_node_accept_children(node->children, visitor);
    printf("}\n\n");
}

void
graphprinter_visit_param_list (struct _Visitor *visitor, struct AstNode *node)
{
    is_symboldecl = TRUE;
    printf("\tnode_%x -> node_%x;\n", node->parent, node);
    printf("\tnode_%x [label=\"%s\",style=", node, node->name);
    printf("filled,color=\"#22DDAA\",fillcolor=\"#EEFFEE\"];\n");
    printf("\nsubgraph cluster_%x {\n\tstyle=dotted;\n", node);
    ast_node_accept_children(node->children, visitor);
    printf("}\n\n");
}

void
graphprinter_visit_statement_list (struct _Visitor *visitor, struct AstNode *node)
{
    is_symboldecl = FALSE;
    printf("\tnode_%x -> node_%x;\n", node->parent, node);
    printf("\tnode_%x [label=\"%s\",style=", node, node->name);
    printf("filled,color=\"#22DDAA\",fillcolor=\"#EEFFEE\"];\n");
    printf("\nsubgraph cluster_%x {\n\tstyle=dotted;\n", node);
    ast_node_accept_children(node->children, visitor);
    printf("}\n\n");
}

void
graphprinter_visit_binary_expr (struct _Visitor *visitor, struct AstNode *node)
{
    printf("\tnode_%x -> node_%x;\n", node->parent, node);
    printf("\tnode_%x [label=\"%s\\n(%s)\",style=",
           node, node->name, node->children->sibling->name);
    printf("filled,fillcolor=\"#EEFFEE\",color=\"#%s\"];\n",
           (node->type == ERROR) ? "FF0000" : "EEFFEE");
    ast_node_accept_children(node->children, visitor);
}

void
graphprinter_visit_callparam_list (struct _Visitor *visitor, struct AstNode *node)
{
    printf("\tnode_%x -> node_%x;\n", node->parent, node);
    printf("\tnode_%x [label=\"%s\",style=", node, node->name);
    printf("filled,color=\"#22DDAA\",fillcolor=\"#EEFFEE\"];\n");
    //printf("\nsubgraph cluster_%x {\n\tstyle=dotted;\n", node);
    ast_node_accept_children(node->children, visitor);
    //printf("}\n\n");
}

void
graphprinter_visit_call (struct _Visitor *visitor, struct AstNode *node)
{
    printf("\tnode_%x -> node_%x;\n", node->parent, node);
    printf("\tnode_%x [label=\"%s\",style=", node, node->name);
    printf("filled,fillcolor=\"#EEFFEE\",color=\"#%s\"];\n",
           (node->type == ERROR) ? "FF0000" : "EEFFEE");
    printf("\nsubgraph cluster_%x {\n\tstyle=dotted;\n", node);
    ast_node_accept_children(node->children, visitor);
    printf("}\n\n");
}

void
graphprinter_visit_identifier (struct _Visitor *visitor, struct AstNode *node)
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
    ast_node_accept_children(node->children, visitor);
}

void
graphprinter_visit_literal (struct _Visitor *visitor, struct AstNode *node)
{
    printf("\tnode_%x -> literal_%x;\n", node->parent, node);
    printf("\tliteral_%x [label=\"", node);
    value_print(stdout, &node->value, node->type);
    printf("\",style=filled,color=\"#FFFFCC\"];\n");
    ast_node_accept_children(node->children, visitor);
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
                 self->kind == PROGRAM_DECL ||
                 strstr(self->name, "List");

    printf("\tnode_%x [label=\"%s\",style=", self, self->name);

    if (is_cluster) {
        if (is_program)
            printf("filled,color=orange,fillcolor=\"#FFEEEE\"];\n");
        else if (is_funcproc)
            printf("filled,color=blue,fillcolor=\"#EEEEFF\"];\n");
        else
            printf("filled,color=\"#22DDAA\",fillcolor=\"#EEFFEE\"];\n");
        printf("\nsubgraph cluster_%x {\n\tstyle=dotted;\n", self, self->name);
    } else
        printf("filled,color=\"#EEFFEE\"];\n");

    if (is_funcproc) {
        printf("\n\tsubgraph cluster_symtab_0x%x {\t\tstyle=filled;\n", self->symbol);
        printf("\t\tcolor=\"#EFEFEF\";\n\t\tfontname=Courier;\n");
        printf("\t\tnode [style=filled,color=white,fillcolor=\"#CCFF99\"];\n");
        //_ast_node_print_graph_symbol_table(self->symbol);
        printf("\t}\n\n");
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
            value_print(stdout, &self->value, self->type);
            printf("\",style=filled,color=\"#FFFFCC\"];\n");
            printf("\tnode_%x -> literal_%x;\n", self, self);
        }

    }

    _print_graph(self->children);

    if (is_cluster)
        printf("}\n\n");

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

    printf("\n\tsubgraph cluster_symtab_%x {\n\t\tstyle=filled;\n", node);

    if (node->parent == NULL)
        printf("\t\tcolor=orange;\n");
    else
        printf("\t\tcolor=blue;\n");

    printf("\t\tstyle=filled;\n\t\tfillcolor=\"#EFEFEF\";\n\t\tfontname=Courier;\n");
    printf("\t\tnode [style=filled,color=white,fillcolor=\"#CCFF99\"];\n");

    _print_symbols(node->symbol->next);

    printf("\t}\n\n");
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

