#include <stdio.h>
#include <stdlib.h>
#include "base.h"

Type
type_get_from_lexeme(const char *lexeme)
{
    if (!strcmp (lexeme, "Integer"))
        return INTEGER;
    else if (!strcmp (lexeme, "Boolean"))
        return BOOLEAN;
    else if (!strcmp (lexeme, "Char"))
        return CHAR;
    else
        return NONE_TYPE;
}

void
value_print(Value *value, Type type)
{
    if (type == INTEGER) {
        printf("%d", value->integer);
    } else if (type == BOOLEAN) {
        printf("%d", value->boolean);
    } else if (type == CHAR) {
        printf("%c", value->character);
    } else {
        fprintf(stderr, "base.c: value_print: unknow type\n");
        exit(1);
    }
}

void
value_get(Value *value, Type type, void *val)
{
    if (value == NULL) {
        fprintf(stderr, "base.c: value_get: value == NULL\n");
        exit(1);
    }

    if (type == INTEGER) {
       *((int *) val) = value->integer;
    } else if (type == BOOLEAN) {
        *((bool *) val) = value->boolean;
    } else if (type == CHAR) {
        *((char *) val) = value->character;
    } else {
        fprintf(stderr, "base.c: value_get: unknow type\n");
        exit(1);
    }
}

void
value_set(Value *value, Type type, void *val)
{
    if (value == NULL) {
        fprintf(stderr, "base.c: value_set: value == NULL\n");
        exit(1);
    }

    if (type == NONE_TYPE || val == NULL) {
        value->integer = 0;
    } else if (type == INTEGER) {
       value->integer = *((int *) val);
    } else if (type == BOOLEAN) {
       value->boolean = *((bool *) val);
    } else if (type == CHAR) {
       value->character = *((char *) val);
    } else {
       fprintf(stderr, "base.c: value_set: unknow type\n");
       exit(1);
    }
}

void
value_set_from_int(Value *value, int val)
{
    value_set(value, INTEGER, VOID(val));
}

void
value_set_from_bool(Value *value, bool val)
{
    value_set(value, BOOLEAN, VOID(val));
}

void
value_set_from_char(Value *value, char val)
{
    value_set(value, CHAR, VOID(val));
}

