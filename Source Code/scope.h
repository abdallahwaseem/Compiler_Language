#include "symbolTable.h"

struct scope
{
    struct variable_entry *my_table ; // current table in this scope
    struct scope *my_parent ;      // ptr to my parent scope
};

// since we cant initialize inside the struct 
// we will make an initializer function that initialize struct and return it
struct scope initialize_scope()
{
    struct scope my_scope;
    my_scope.my_table = NULL;
    my_scope.my_parent = NULL;
    return my_scope;
}

// setting parent of a scope
void set_parent_of_scope(struct scope* my_scope, struct scope* in_my_parent)
{
    my_scope->my_parent = in_my_parent;
    return;
}

struct variable_entry *find_variable_in_scope(struct scope *my_scope, char *variable_to_find)
{
    struct variable_entry *variable_found = NULL;

    // first check if its in the current scope
    variable_found = find_variable_in_symbolTable(&my_scope->my_table, variable_to_find);
    if (variable_found != NULL)
    {
        return variable_found;
    }
    // if its not in the current scope check our parents iteratively till we reach null
    struct scope *upper_scopes = my_scope->my_parent;
    while (upper_scopes != NULL)
    {
        variable_found = find_variable_in_symbolTable(&upper_scopes->my_table, variable_to_find);
        if (variable_found != NULL)
        {
            return variable_found;
        }
        else
        {
            // go up one more level
            upper_scopes = upper_scopes->my_parent;
        }
    }
    return NULL;
}


RETURN_CODES add_variable_to_scope(struct scope *my_scope, char *name, int is_init, DataTypes datatype)
{
    // making a new variable
    struct variable_entry *new_variable = (struct variable_entry *)malloc(sizeof(struct variable_entry));

    // setting the variable info
    new_variable->is_initialized = is_init;
    new_variable->is_used = 0;
    new_variable->my_datatype = datatype;
    new_variable->variable_name = name;
    return add_variable_to_symbolTable(&my_scope->my_table, new_variable);
}

RETURN_CODES set_variable_used_in_scope(struct scope *my_scope, char *variable_name)
{
    // calling the function in the symboltable file
    return set_variable_used(&my_scope->my_table, variable_name);
}

RETURN_CODES assign_previously_declared_variable_in_scope(struct scope *my_scope, char *variable_name)
{
    // calling the function in the symboltable file
    return assign_previously_declared_variable(&my_scope->my_table, variable_name);
}

