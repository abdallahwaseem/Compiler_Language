#pragma once
#ifndef _STRUCTS
#define _STRUCTS
#include <stdlib.h>
#include "enums.h"

struct lexemeInfo
{
    int intValue;
    float floatValue;
    char charValue;
    char *stringValue;
    int boolValue;
    char *variableName;
    int is_initialized; // used for identifiers 0: uninit, 1 : init
    DataTypes my_type;
};

void set_lexemeInfo(struct lexemeInfo **input_lexeme, DataTypes my_type)
{
    (*input_lexeme) = (struct lexemeInfo *)malloc(sizeof(struct lexemeInfo));
    (*input_lexeme)->my_type = my_type;
    (*input_lexeme)->is_initialized = 1; // initially 1 till its  assigned by zero if identifer
}

// linked list of arguments
struct argument_info
{
    DataTypes my_type;
    char *my_name;
    struct argument_info *next_arg; // points to next arguments in linked list
};

#endif