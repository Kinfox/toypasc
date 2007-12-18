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

Symbol *symbol_table_new(void);
void symbol_table_destroy(void);
char *symbol_table_dump(void);
Symbol *symbol_insert(char const * name, int type);
Symbol *symbol_lookup(char const * name);
int symbol_exists(char const * name);

#endif // SYMBOL_TABLE_H
