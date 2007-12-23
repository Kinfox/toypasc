#ifndef ABSTRACT_VISITOR_H
#define ABSTRACT_VISITOR_H

#include "ast.h"

//typedef void (*VisitFunc)(struct AstNode *);

typedef struct {
    void (*visit_program)(struct AstNode *);
    void (*visit_program_declaration)(struct AstNode *);
    /*VisitFunc *visit_var_declaration;
    VisitFunc *visit_identifier_list;
    VisitFunc *visit_identifier;
    VisitFunc *visit_procedure;
    VisitFunc *visit_function;
    VisitFunc *visit_param_list;*/
} Visitor;

#endif // ABSTRACT_VISITOR_H
