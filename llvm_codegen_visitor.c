#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "llvm_codegen_visitor.h"

static char *pf_name;
static int tmp_var = 0;

static void _tab(struct AstNode *node);
static int _get_type_size(Type type);
static char *_create_temporary();
static void _print_op_symbol(struct AstNode *node);

Visitor *
llvm_codegen_new()
{
    Visitor *visitor = (Visitor *) malloc (sizeof(Visitor));

    visitor->visit_program = &llvm_codegen_visit_program;
    visitor->visit_programdecl = &llvm_codegen_visit_programdecl;
    visitor->visit_vardecl_list = &llvm_codegen_visit_vardecl_list;
    visitor->visit_vardecl = &llvm_codegen_visit_vardecl;
    visitor->visit_identifier_list = &llvm_codegen_visit_identifier_list;
    visitor->visit_procfunc_list = &llvm_codegen_visit_procfunc_list;
    visitor->visit_procedure = &llvm_codegen_visit_procfunc;
    visitor->visit_function = &llvm_codegen_visit_procfunc;
    visitor->visit_param_list = &llvm_codegen_visit_param_list;
    visitor->visit_parameter = &llvm_codegen_visit_parameter;
    visitor->visit_statement_list = &llvm_codegen_visit_statement_list;
    visitor->visit_printint_stmt = &llvm_codegen_visit_printint_stmt;
    visitor->visit_printchar_stmt = &llvm_codegen_visit_printchar_stmt;
    visitor->visit_printbool_stmt = &llvm_codegen_visit_printbool_stmt;
    visitor->visit_printline_stmt = &llvm_codegen_visit_printline_stmt;
    visitor->visit_assignment_stmt = &llvm_codegen_visit_assignment_stmt;
    visitor->visit_if_stmt = &llvm_codegen_visit_if_stmt;
    visitor->visit_while_stmt = &llvm_codegen_visit_while_stmt;
    visitor->visit_for_stmt = &llvm_codegen_visit_for_stmt;
    visitor->visit_rel_expr = &llvm_codegen_visit_binary_expr;
    visitor->visit_add_expr = &llvm_codegen_visit_binary_expr;
    visitor->visit_mul_expr = &llvm_codegen_visit_binary_expr;
    visitor->visit_notfactor = &llvm_codegen_visit_notfactor;
    visitor->visit_call = &llvm_codegen_visit_call;
    visitor->visit_callparam_list = &llvm_codegen_visit_callparam_list;
    visitor->visit_identifier = &llvm_codegen_visit_identifier;
    visitor->visit_literal = &llvm_codegen_visit_literal;
    visitor->visit_add_op = &llvm_codegen_visit_binary_op;
    visitor->visit_mul_op = &llvm_codegen_visit_binary_op;
    visitor->visit_rel_op = &llvm_codegen_visit_binary_op;
    visitor->visit_not_op = &llvm_codegen_visit_not_op;

    return visitor;
}

void
llvm_codegen_visit_program(struct _Visitor *visitor, struct AstNode *node)
{
    struct AstNode *child;

    printf("; Generated with toypasc\n");
    for (child = node->children;
         child != NULL && child->kind != STATEMENT_LIST;
         child = child->sibling) {
        ast_node_accept(child, visitor);
    }

    if (child != NULL) {
        printf("; Definition of main function\n");
        printf("define i32 @main () {\n");
        ast_node_accept(child, visitor);
        printf(TAB"ret i32 0\n}\n\n");
    }
}

void
llvm_codegen_visit_programdecl(struct _Visitor *visitor, struct AstNode *node)
{
    printf("; program ");
    ast_node_accept(node->children, visitor);
    printf("\n\n");
    printf("; Declare the string constants as a global constants...\n");
    printf("@.true_str = internal constant [5 x i8] c\"true\\00\"\n");
    printf("@.false_str = internal constant [6 x i8] c\"false\\00\"\n");
    printf("@.int_fmt = internal constant [3 x i8] c\"%%d\\00\"\n");

    printf("\n; External declaration of functions\n");
    printf("declare i32 @puts(i8 *)\n");
    printf("declare i32 @putchar(i32)\n");
    printf("declare i32 @printf(i8*, ...)\n\n");
}

void
llvm_codegen_visit_vardecl_list (struct _Visitor *visitor, struct AstNode *node)
{
    ast_node_accept_children(node->children, visitor);
    printf("\n");
}

void
llvm_codegen_visit_identifier_list (struct _Visitor *visitor, struct AstNode *node)
{
    struct AstNode *child;

    for (child = node->children; child != NULL; child = child->sibling) {
        ast_node_accept(child, visitor);
        if (child->sibling != NULL)
            printf(", ");
    }
}

void
llvm_codegen_visit_procfunc_list (struct _Visitor *visitor, struct AstNode *node)
{
    ast_node_accept_children(node->children, visitor);
}

void
llvm_codegen_visit_procfunc (struct _Visitor *visitor, struct AstNode *node)
{
    struct AstNode *child;

    pf_name = _create_temporary();

    printf("define ");
    PRINT_TYPE(node->type);
    printf(" ");

    child = node->children; // Identifier
    ast_node_accept(child, visitor);

    printf(" ( ");

    child = child->sibling;
    if (child->kind == PARAM_LIST) {
        ast_node_accept(child, visitor);
        child = child->sibling;
    }

    printf(" ) {\n");
    printf("entry:\n");

    if (node->kind == FUNCTION) {
        printf(TAB);
        PRINT_TYPE(node->type);
        printf(" %%%s\n", pf_name);
    }

    if (child->kind == VARDECL_LIST) {
        ast_node_accept(child, visitor);
        child = child->sibling;
    }

    printf("\n");

    ast_node_accept(child, visitor);

    printf(TAB"ret ");
    PRINT_TYPE(node->type);

    if (node->kind == FUNCTION)
        printf(" %s", pf_name);

    printf("\n}\n\n");

    free(pf_name);
}

void
llvm_codegen_visit_param_list (struct _Visitor *visitor, struct AstNode *node)
{
    struct AstNode *child;

    for (child = node->children; child != NULL; child = child->sibling) {
        //printf("%s ", _get_type_string(child->type));
        ast_node_accept(child, visitor);
        if (child->sibling != NULL)
            printf(", ");
    }
}

void
llvm_codegen_visit_statement_list (struct _Visitor *visitor, struct AstNode *node)
{
    struct AstNode *child;

    for (child = node->children; child != NULL; child = child->sibling) {
        _tab(child);
        ast_node_accept(child, visitor);
        printf("\n");
    }
}

void
llvm_codegen_visit_binary_expr (struct _Visitor *visitor, struct AstNode *node)
{
    struct AstNode *left = node->children;
    struct AstNode *op = left->sibling;
    struct AstNode *right = op->sibling;

    fprintf(stderr, "[%s]\n", node->name);
    fprintf(stderr, "left : %s\n", left->name);
    fprintf(stderr, "op   : %s\n", op->name);
    fprintf(stderr, "right: %s\n\n", right->name);

    if (IS_LITERAL(left->kind) && IS_LITERAL(right->kind)) {
        _print_op_symbol(op);
        printf(" ");
        ast_node_accept(left, visitor);
        printf(", ");
        ast_node_accept(right, visitor);
    }

    // %tmp = mul i32 %x, %y
    // %tmp2 = add i32 %tmp, %z
    //ast_node_accept_children(node->children, visitor);
}

void
llvm_codegen_visit_callparam_list (struct _Visitor *visitor, struct AstNode *node)
{
    ast_node_accept(node->children, visitor);
}

void
llvm_codegen_visit_identifier (struct _Visitor *visitor, struct AstNode *node)
{
    printf("%s%s", node->symbol->is_global ? "@" : TAB"%",
                   node->symbol->name);
}

void
llvm_codegen_visit_literal (struct _Visitor *visitor, struct AstNode *node)
{
    if (node->type == BOOLEAN) {
        printf("%bool_str = getelementptr [");
        if (node->value.boolean)
            printf("5 x i8]* @.true");
        else
            printf("6 x i8]* @.false");
        printf("_str, i32 0, i32 0\n");

    } else
        printf("%d", node->value.integer);
}

void
llvm_codegen_visit_vardecl (struct _Visitor *visitor, struct AstNode *node)
{
    struct AstNode *child;
    const char *type;// = _get_type_string(node->type);

    child = node->children;
    child->visited = TRUE;

    for (child = child->children; child != NULL; child = child->sibling) {
        ast_node_accept(child, visitor);
        if (node->parent->parent->kind == PROGRAM)
            printf(" = global i%d 0\n", _get_type_size(child->type));
        else
            printf(" = add i%d 0, 0\n", _get_type_size(child->type));
    }
}

void
llvm_codegen_visit_parameter (struct _Visitor *visitor, struct AstNode *node)
{
    PRINT_TYPE(node->type);
    printf(" ");
    ast_node_accept(node->children, visitor);
}

void
llvm_codegen_visit_printint_stmt (struct _Visitor *visitor, struct AstNode *node)
{
    printf("call i32 (i8* noalias , ...)* bitcast (i32 (i8*, ...)* ");
    printf("@printf to i32 (i8* noalias , ...)*)( i8* getelementptr ");
    printf("([3 x i8]* @.int_fmt, i32 0, i32 0) noalias , i32 ");
    ast_node_accept(node->children, visitor);
    printf(" )\n");
}

void
llvm_codegen_visit_printchar_stmt (struct _Visitor *visitor, struct AstNode *node)
{
    printf("call i32 @putchar( i32 ");
    ast_node_accept(node->children, visitor);
    printf(" )\n");
}

void
llvm_codegen_visit_printbool_stmt (struct _Visitor *visitor, struct AstNode *node)
{
    ast_node_accept(node->children, visitor);
    printf(TAB"call i32 @puts( i8 * %bool_str )\n");
}

void
llvm_codegen_visit_printline_stmt (struct _Visitor *visitor, struct AstNode *node)
{
    printf("call i32 @putchar( i32 10 )\n");
}

void
llvm_codegen_visit_assignment_stmt (struct _Visitor *visitor, struct AstNode *node)
{
    Symbol *lsym;
    int tsize = _get_type_size(node->children->type);
    printf("; [Template] store i32 50, i32* @x, align 4\n");

    lsym = node->children->symbol;

    fprintf(stderr, "%s - ", node->children->symbol->name);
    fprintf(stderr, "is_global? %d\n", node->children->symbol->is_global);

    //printf(TAB"store i%d ", tsize);
    //ast_node_accept(node->children->sibling, visitor);
    //printf(", i%d* ", tsize);
    ast_node_accept(node->children, visitor);
    printf("\n");
}

void
llvm_codegen_visit_if_stmt (struct _Visitor *visitor, struct AstNode *node)
{
    struct AstNode *child;
    const char *var;

    printf("if (");
    child = node->children; // Expression
    ast_node_accept(child, visitor);
    printf(") {\n");

    child = child->sibling; // If Statements
    ast_node_accept(child, visitor);

    printf("\n");
    _tab(node);
    printf("}");

    child = child->sibling; // Else Statements

    if (child != NULL) {
        printf(" else {\n");
        ast_node_accept(child, visitor);
        printf("\n");
        _tab(node);
        printf("}");
    }
    printf("\n");
}

void
llvm_codegen_visit_while_stmt (struct _Visitor *visitor, struct AstNode *node)
{
    struct AstNode *child;
    const char *var;

    printf("while (");
    child = node->children; // Expression
    ast_node_accept(child, visitor);
    printf(") {\n");

    child = child->sibling; // Statements
    ast_node_accept(child, visitor);

    _tab(node);
    printf("}\n");
}

void
llvm_codegen_visit_for_stmt (struct _Visitor *visitor, struct AstNode *node)
{
    struct AstNode *child;
    const char *var;

    printf("for (");
    child = node->children; // Assignment
    ast_node_accept(child, visitor);

    var = child->children->symbol->name;
    printf(" %s < ", var);

    child = child->sibling; // Stop condition
    ast_node_accept(child, visitor);

    printf("; %s++) {\n", var);

    child = child->sibling; // Statements
    ast_node_accept_children(child, visitor);

    printf("\n");
    _tab(node);
    printf("}\n");
}

void
llvm_codegen_visit_notfactor (struct _Visitor *visitor, struct AstNode *node)
{
    ast_node_accept_children(node->children, visitor);
}

void
llvm_codegen_visit_call (struct _Visitor *visitor, struct AstNode *node)
{
    struct AstNode *child = node->children;

    printf("call ");
    PRINT_TYPE(child->symbol->type);
    printf(" ");
    ast_node_accept(child, visitor);
    printf("( ");
    for (child = child->sibling->children; child != NULL;
         child = child->sibling) {
        PRINT_TYPE(child->type);
        printf(" ");
        ast_node_accept(child, visitor);
        if (child->sibling != NULL)
            printf(", ");
    }
    printf(" )\n");
}

void
llvm_codegen_visit_simplenode (struct _Visitor *visitor, struct AstNode *node)
{
    ast_node_accept_children(node->children, visitor);
}

void
llvm_codegen_visit_binary_op (struct _Visitor *visitor, struct AstNode *node)
{
    _print_op_symbol(node);
}

void
llvm_codegen_visit_not_op (struct _Visitor *visitor, struct AstNode *node)
{
    printf(" !", node->name);
}

static void
_tab(struct AstNode *node) {
    struct AstNode *parent;
    for (parent = node->parent; parent->parent != NULL; parent = parent->parent)
        printf(TAB);
}

static int
_get_type_size(Type type)
{
    switch (type) {
        case INTEGER:
            return 32;
        case BOOLEAN:
            return 1;
        case CHAR:
            return 8;
        default:
            return 0;
    }
}

static char
*_create_temporary()
{
    char *temp;

    if (asprintf (&temp, "tmp%d", tmp_var) < 0)
        return NULL;

    tmp_var++;
    return temp;
}

static void
_print_op_symbol(struct AstNode *node)
{
    switch (node->kind) {
        /*case T_OR:
            printf(" || ");
            break;
        case T_AND:
            printf(" && ");
            break;
        case T_EQUAL:
            printf(" == ");
            break;
        case T_NOTEQUAL:
            printf(" != ");
            break;
        case T_LESSER:
            printf(" < ");
            break;
        case T_GREATER:
            printf(" > ");
            break;
        case T_LESSEREQUAL:
            printf(" <= ");
            break;
        case T_GREATEREQUAL:
            printf(" >= ");
            break;*/
        case T_PLUS:
            printf("add");
            break;
        case T_MINUS:
            printf("sub");
            break;
        case T_STAR:
            printf("mul");
            break;
        /*case T_SLASH:
            printf(" %s ", node->name);*/
    }
}
