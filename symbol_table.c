#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "symbol_table.h"


Symbol *
symbol_insert(Symbol * table, char const * name, int type)
{
    Symbol *symbol;

    symbol = (Symbol *) malloc (sizeof(Symbol));
    symbol->next = table;
    symbol->name =  (char *) malloc (sizeof(char)*strlen(name));
    strcpy (symbol->name, name);
    symbol->type = type;
    return symbol;
    
}

Symbol *
symbol_lookup(Symbol * table, char const * name)
{
    Symbol *temp;
    temp = table;
    
    while(temp != NULL){
    
        if(strcmp (temp->name, name)){
    
            return temp;   
    
        }
    
        temp = temp->next;  
    
    }
    
    return temp;

}

void
symbol_table_destroy(Symbol *table)
{
    Symbol *first;
    Symbol *second;
    first = table;

    while(first != NULL){
        second = first;
        first = first->next;
        free(second);
    }

    free(first);
    free(second);
}

void
symbol_print(Symbol *symbol)
{
    if (symbol == NULL) {
        printf("NULL\n");
        return;
    }
    
    printf("Symbol: %x\n", symbol);
    printf("%s\n", symbol->name);
    printf("%d\n", symbol->type);
}

void
symbol_table_dump(Symbol *table)
{
    Symbol *temp;
    temp = table;

    while(temp != NULL) {
        symbol_print(temp);
        temp = temp->next;
    }

    free(temp);
}
