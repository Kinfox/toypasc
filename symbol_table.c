#include <string.h>
#include "symbol_table.h"

void
symbol_table_destroy(Symbol * table)
{
    Symbol * first, second;
    first = table;
        
    while(first != NULL){
    
        second = first;
        first = first->next;
        free(second);
            
    }
    
    free(first);
    free(second);

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
     
    while(strcmp (temp->name,name) && temp != NULL){
    
        temp = temp->next;
    
    }
    
    return temp;
    
}

int
symbol_exists(Symbol * table, char const * name)
{

    Symbol * temp;
    temp = table;
     
    while(temp != NULL){
    
        if(strcmp (temp->name,name)){
        
            return 1;
        
        }
    
    }
        
    return 0;

}


