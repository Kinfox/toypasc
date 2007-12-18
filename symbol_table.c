#include <string.h>
#include "symbol_table.h"

void
symbol_table_destroy(Symbol * table)
{
    Symbol * prev, next;
    prev = table;
    next = table->next;
    
    while(temp->next != NULL){
    
        free(prev);
        prev = next->next;
        free(next);
        next = prev->next;
    
    }

}

char *
symbol_table_dump(Symbol table)
{

    Symbol * temp;
    temp = table;
     
    while(temp =! NULL){
    
        printf("%s\n", temp->name);
        printf("%d\n", temp->type);
        temp = temp->next;
    
    }
    
    free(temp);   

}

Symbol *
symbol_insert(Symbol * table, char const * name, int type)
{

    Symbol * symbol = (Symbol *) malloc (sizeof(Symbol));
    symbol->next = table;
    strcpy (symbol->name,name);
    symbol->type = type;
    return symbol;

}

Symbol *
symbol_lookup(Symbol * table, char const * name)
{
    
    Symbol * temp;
    temp = table;
     
    while(temp =! NULL){
    
        printf("%s\n", temp->name);
        printf("%d\n", temp->type);
        temp = temp->next;
    
    }
    
    free(temp);
    
}

int
symbol_exists(char const * name)
{
}


