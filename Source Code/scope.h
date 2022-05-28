#include "symbolTable.h"

struct scope
{
    struct variable_entry *my_table; // current table in this scope
    struct scope *my_parent;         // ptr to my parent scope
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
void set_parent_of_scope(struct scope *my_scope, struct scope *in_my_parent)
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

RETURN_CODES add_variable_to_scope(struct scope *my_scope, char *name, int is_init, DataTypes datatype, Kind kind,
                                   DataTypes *input_params, int no_of_params)
{
    // making a new variable
    struct variable_entry *new_variable = (struct variable_entry *)malloc(sizeof(struct variable_entry));

    // setting the variable info
    new_variable->is_initialized = is_init;
    new_variable->is_used = 0;
    new_variable->my_kind = kind;
    new_variable->my_datatype = datatype;
    new_variable->variable_name = name;
    new_variable->params = input_params;
    new_variable->no_of_params = no_of_params;
    return add_variable_to_symbolTable(&my_scope->my_table, new_variable);
}

RETURN_CODES set_variable_used_in_scope(struct scope *my_scope, char *variable_name)
{
    struct variable_entry *variable_found = NULL;
    // first check if its in the current scope
    variable_found = find_variable_in_symbolTable(&my_scope->my_table, variable_name);
    if (variable_found != NULL)
    {
        // therefore we found it
        return set_variable_used(&my_scope->my_table, variable_found);
    }
    // if its not in the current scope check our parents iteratively till we reach null
    struct scope *upper_scopes = my_scope->my_parent;
    while (upper_scopes != NULL)
    {
        variable_found = find_variable_in_symbolTable(&upper_scopes->my_table, variable_name);
        if (variable_found != NULL)
        {
            return set_variable_used(&upper_scopes->my_table, variable_found);
        }
        else
        {
            // go up one more level
            upper_scopes = upper_scopes->my_parent;
        }
    }
    return FAILURE;
}


RETURN_CODES assign_previously_declared_variable_in_scope(struct scope *my_scope, char *variable_to_find)
{
    struct variable_entry *variable_found = NULL;

    // first check if its in the current scope
    variable_found = find_variable_in_symbolTable(&my_scope->my_table, variable_to_find);
    if (variable_found != NULL)
    {
        // therefore we found it
        return assign_previously_declared_variable(&my_scope->my_table, variable_found);
    }
    // if its not in the current scope check our parents iteratively till we reach null
    struct scope *upper_scopes = my_scope->my_parent;
    while (upper_scopes != NULL)
    {
        variable_found = find_variable_in_symbolTable(&upper_scopes->my_table, variable_to_find);
        if (variable_found != NULL)
        {
            return assign_previously_declared_variable(&upper_scopes->my_table, variable_found);
        }
        else
        {
            // go up one more level
            upper_scopes = upper_scopes->my_parent;
        }
    }
    return FAILURE;
}

void delete_all(struct scope *my_scope)
{
    struct variable_entry *current_entry, *tmp;

    HASH_ITER(hh, my_scope->my_table, current_entry, tmp)
    {
        HASH_DEL(my_scope->my_table, current_entry); /* delete; users advances to next */
        free(current_entry);                         /* optional- if you want to free  */
    }
}

struct scope *delete_scope(struct scope *my_scope)
{
    delete_all(my_scope);
    return my_scope->my_parent;
}

void print_symbol_table_in_scope(struct scope *my_scope)
{
    print_symbol_table(&my_scope->my_table);
}