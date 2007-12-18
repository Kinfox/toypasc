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

void symbol_table_destroy(Symbol * table, );
char *symbol_table_dump(Symbol * table, );
Symbol *symbol_insert(Symbol * table, char const * name, int type);
Symbol *symbol_lookup(Symbol * table, char const * name);
int symbol_exists(Symbol * table, char const * name);

#endif // SYMBOL_TABLE_H
