#ifndef SIMPLEPRINTER_VISITOR_H
#define SIMPLEPRINTER_VISITOR_H

#include "ast.h"

Visitor *simpleprinter_new();

void simpleprinter_visit (struct AstNode *node);

#endif // SIMPLEPRINTER_VISITOR_H
