#include <string.h>
#include "symbol_table.h"

void
symbol_table_destroy(Symbol * table)
{
    Symbol * temp;

    while(table->next != NULL){
    
        table
    
    }

}

char *
symbol_table_dump(void)
{

    

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
symbol_lookup(char const * name)
{

        
    
}

int
symbol_exists(char const * name)
{
}


