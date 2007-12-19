#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "base.h"

#define symbol_table_init()     sym_table = NULL
#define symbol_add(table, name) table = symbol_insert(table, name)

typedef struct _symbol {
    char *name;
    Type type;
    Value value;
    //Scope
    struct _symbol *next;
} Symbol;

static Symbol *sym_table;

Symbol *symbol_new(char const * name);
Symbol *symbol_insert(Symbol *table, char const *name);
Symbol *symbol_lookup(Symbol *table, char const *name);
//symbol_get_scope, parent_scope, etc....

void symbol_table_destroy(Symbol **table);
void symbol_table_dump(Symbol *table);
void symbol_print(Symbol *symbol);

#endif // SYMBOL_TABLE_H
