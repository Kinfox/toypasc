#ifndef BASE_H
#define BASE_H

#define VOID(var)   ((void *) &var)

typedef enum {
    FALSE = 0,
    TRUE
} bool;

typedef enum TypeEnum {
    NONE_TYPE = -1,
    INTEGER,
    BOOLEAN,
    CHAR,
    PROCEDURE,
    FUNCTION
} Type;

typedef union {
        int integer;
        bool boolean;
        char character;
} Value;

Type type_get_from_lexeme(const char *lexeme);

void value_print(Value *value, Type type);
void value_get(Value *value, Type type, void *val);
void value_set(Value *value, Type type, void *val);
void value_set_from_int(Value *value, int val);
void value_set_from_bool(Value *value, bool val);
void value_set_from_char(Value *value, char val);

#endif // BASE_H

