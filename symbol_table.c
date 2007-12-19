#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "symbol_table.h"

Symbol *
symbol_new(char const * name)
{
    Symbol * symbol = (Symbol *) malloc (sizeof(Symbol));
    symbol->type = NONE_TYPE;
    value_set(&symbol->value, symbol->type, NULL);
    symbol->next = NULL;

    if (name != NULL)
        symbol->name = strdup(name);
     else
        symbol->name = NULL;

    return symbol;
}


Symbol *
symbol_insert(Symbol **table, char const *name)
{
    Symbol *symbol = symbol_lookup(*table, name);

    if (symbol == NULL) {
        symbol = symbol_new(name);
        symbol->next = *table;
        *table = symbol;
    }

    return symbol;
}

Symbol *
symbol_lookup(Symbol *table, char const *name)
{
    Symbol *temp;
    temp = table;

    while (temp != NULL) {
        if (!strcmp (temp->name, name))
            return temp;
        temp = temp->next;
    }

    return temp;
}

void
symbol_table_destroy(Symbol **table)
{
    Symbol *first;
    Symbol *to_kill;
    first = *table;
    *table = NULL;

    while (first != NULL) {
        to_kill = first;
        first = first->next;
        free(to_kill);
    }

    free(first);
}

void
symbol_print(Symbol *symbol)
{
    if (symbol == NULL) {
        printf("NULL\n\n");
        return;
    }

    printf("Symbol: %x\n", symbol);
    printf("name: %s\n", symbol->name);
    printf("type: %d\n", symbol->type);
    printf("value:");
    value_print(&symbol->value, symbol->type);
    printf("\nnext: %x\n\n", symbol->next);
}

void
symbol_table_dump(Symbol *table)
{
    Symbol *temp = table;

    while(temp != NULL) {
        symbol_print(temp);
        temp = temp->next;
    }

    free(temp);
}
