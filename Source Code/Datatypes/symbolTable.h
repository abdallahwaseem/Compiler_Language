#ifndef _SYMTAB
#define _SYMTAB
#include "uthash.h"
#include "structs.h"
#include "symbolTable_entry.h"
#include <stdio.h>

struct variable_entry *find_variable_in_symbolTable(struct variable_entry **symbolTable, char *variable_to_find)
{

    struct variable_entry *variable = NULL;
    HASH_FIND_STR(*symbolTable, variable_to_find, variable);

    if (variable != NULL)
        return variable;
    else
        return NULL;
}

// add a new variable to symboltable
// whether he wrote
// int x; or int x = 10;
// we will save whether he initialized it or not from yacc file
RETURN_CODES add_variable_to_symbolTable(struct variable_entry **symbolTable, struct variable_entry *variable_to_add)
{

    // first we need to check if it exist before

    if (find_variable_in_symbolTable(symbolTable, variable_to_add->variable_name) != NULL)
    {
        // therefore multiple definition
        return FAILURE;
    }

    // FOR CONSTANTS
    // we have special case to handle
    // 1: any constant must be initialized within the same line of declaration
    // const int x;
    // 	x = 10;
    // this is an error so we must check this within declaration
    if (variable_to_add->my_datatype == CONST_INT_DT || variable_to_add->my_datatype == CONST_FLOAT_DT || variable_to_add->my_datatype == CONST_CHAR_DT || variable_to_add->my_datatype == CONST_STRING_DT)
    {
        // if type is constant we will check if not initialized this is error
        if (variable_to_add->is_initialized == 0)
            return CONSTANT_NOT_INITIALIZED;
    }
    // otherwise its safe to add
    HASH_ADD_STR(*symbolTable, variable_name, variable_to_add);
    return SUCCESS;
}

// if he initialized x in that way -> int x;
// x= 10 ; --> should set x to be assigned to avoid sending error of uninitialzied variable
RETURN_CODES set_variable_used(struct variable_entry **symbolTable, struct variable_entry *variable_to_set)
{
    struct variable_entry *new_variable = copy_variable(variable_to_set);
    HASH_DEL(*symbolTable, variable_to_set); /* delete; users advances to next */
    free(variable_to_set);                   /* optional- if you want to free  */

    new_variable->is_used = 1;
    HASH_ADD_STR(*symbolTable, variable_name, new_variable);
    return SUCCESS;
}

// if he initialized x in that way -> int x;
// x= 10 ; --> should set x to be assigned to avoid sending error of uninitialzied variable
RETURN_CODES assign_previously_declared_variable(struct variable_entry **symbolTable, struct variable_entry *variable_to_set)
{
    // we cant assign any previously declared variable which is constant
    // const int x = 10;
    // x = 11; -> error
    // handled successfully in assign_previously_declared_variable function above
    if (variable_to_set->my_datatype == CONST_INT_DT || variable_to_set->my_datatype == CONST_FLOAT_DT || variable_to_set->my_datatype == CONST_CHAR_DT || variable_to_set->my_datatype == CONST_STRING_DT || variable_to_set->my_datatype == CONST_BOOL_DT)
    {
        // if type is constant we cant reassign it
        return CONSTANT_REASSIGNMENT;
    }
    struct variable_entry *new_variable = copy_variable(variable_to_set);
    new_variable->is_initialized = 1;

    HASH_DEL(*symbolTable, variable_to_set); /* delete; users advances to next */
    free(variable_to_set);                   /* optional- if you want to free  */

    HASH_ADD_STR(*symbolTable, variable_name, new_variable);
    return SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////////////////
// CONSTANT HANDLING
// we have 2 cases to handle
// 1: any constant must be initialized within the same line of declaration
// const int x;
// 	x = 10;
// this is an error so we must check this within declaration
// handled successfully in add_variable_to_symbolTable function above

// 2: we cant assign any previously declared variable which is constant
// const int x = 10;
// x = 11; -> error
// handled successfully in assign_previously_declared_variable function above
///////////////////////////////////////////////////////////////////////////////////////////////

void print_symbol_table(struct variable_entry **symbolTable, FILE *sT)
{
    fputs("\nSymbol table\n", sT);

    fputs("variable name \t data_type \t is_initialized \t kind \n", sT);
    struct variable_entry *temp;

    for (temp = *symbolTable; temp != NULL; temp = (struct variable_entry *)(temp->hh.next))
    {
        fprintf(sT, "%s \t %d \t %d \t %d \n", temp->variable_name, temp->my_datatype, temp->is_initialized, temp->my_kind);
    }
    fprintf(sT, "\n");
}

#endif