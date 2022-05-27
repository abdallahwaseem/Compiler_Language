#include "uthash.h"
typedef enum
{
    FUNCTION,
    PARAMETER,
    VARIABLE
} Kind;

typedef enum
{
    SUCCESS,
    FAILURE,
    CONSTANT_NOT_INITIALIZED,
    CONSTANT_REASSIGNMENT
} RETURN_CODES;

typedef enum
{
    INT,
    FLOAT,
    CHAR,
    STRING,
    Const_INT,
    Const_FLOAT,
    Const_CHAR,
    Const_STRING,
    function_Datatype
} DataTypes;

struct function_Datatype
{
    DataTypes *inputs; // array of inputs
    DataTypes *output; // array of outputs
};

struct variable_entry
{
    char *variable_name; // we will use the name of variable as key
    DataTypes my_datatype;
    int is_initialized; // 0 : uninit , 1 : init

    // for unused variables we will give warnings bec its not used through the whole program
    int is_used; // 0 : unused , 1 : used

    UT_hash_handle hh; /* makes this structure hashable */
};

struct variable_entry *find_variable_in_symbolTable(struct variable_entry **symbolTable, char *variable_to_find)
{

    struct variable_entry *variable = NULL;
    HASH_FIND_STR(*symbolTable, "x", variable);

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
    if (variable_to_add->my_datatype == Const_INT || variable_to_add->my_datatype == Const_FLOAT || variable_to_add->my_datatype == Const_CHAR || variable_to_add->my_datatype == Const_STRING)
    {
        // if type is constant we will check if not initialized this is error
        if (variable_to_add->is_initialized == 0)
            return CONSTANT_NOT_INITIALIZED;
    }
    // otherwise its safe to add
    HASH_ADD_STR(*symbolTable, variable_name, variable_to_add);
    return SUCCESS;
}

RETURN_CODES set_variable_used(struct variable_entry **symbolTable, char *variable_name)
{

    struct variable_entry *variable_to_set = find_variable_in_symbolTable(symbolTable, variable_name);
    if (variable_to_set == NULL)
        return FAILURE;

    variable_to_set->is_used = 1;

    return SUCCESS;
}

// if he initialized x in that way -> int x;
// x= 10 ; --> should set x to be assigned to avoid sending error of uninitialzied variable
RETURN_CODES assign_previously_declared_variable(struct variable_entry **symbolTable, char *variable_name)
{
    struct variable_entry *variable_to_set = find_variable_in_symbolTable(symbolTable, variable_name);
    if (variable_to_set == NULL)
        return FAILURE;

    // we cant assign any previously declared variable which is constant
    // const int x = 10;
    // x = 11; -> error
    // handled successfully in assign_previously_declared_variable function above
    if (variable_to_set->my_datatype == Const_INT || variable_to_set->my_datatype == Const_FLOAT || variable_to_set->my_datatype == Const_CHAR || variable_to_set->my_datatype == Const_STRING)
    {
        // if type is constant we cant reassign it
        return CONSTANT_REASSIGNMENT;
    }

    variable_to_set->is_initialized = 1;
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
