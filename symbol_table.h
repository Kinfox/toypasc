#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

typedef struct _symbol {
    char *name;
    int type;

    union {
        int integer;
        int boolean;
        char character;
    } value;

    struct _symbol *next;
} Symbol;

Symbol *symbol_insert(Symbol *table, char const *name, int type);
Symbol *symbol_lookup(Symbol *table, char const *name);
void symbol_table_destroy(Symbol **table);
void symbol_print(Symbol *symbol);
void symbol_table_dump(Symbol *table);

#endif // SYMBOL_TABLE_H
