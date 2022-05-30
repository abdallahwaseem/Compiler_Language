#include "uthash.h"
#include "enums.h"

struct variable_entry
{
    char *variable_name; // we will use the name of variable as key
    DataTypes my_datatype;
    // for variables, it will be datatypes
    // for functions, it will be ouput

    int is_initialized; // 0 : uninit , 1 : init
    Kind my_kind;       // function , parameter , variable
    // for unused variables we will give warnings bec its not used through the whole program
    int is_used;       // 0 : unused , 1 : used
    DataTypes *params; // the input params to function
    int no_of_params;
    UT_hash_handle hh; /* makes this structure hashable */
};

struct variable_entry *copy_variable(struct variable_entry *old_variable)
{
    // making a new variable
    struct variable_entry *new_variable = (struct variable_entry *)malloc(sizeof(struct variable_entry));
    // setting the variable info
    new_variable->is_initialized = old_variable->is_initialized;
    new_variable->is_used = old_variable->is_used;
    new_variable->my_kind = old_variable->my_kind;
    new_variable->my_datatype = old_variable->my_datatype;
    new_variable->variable_name = old_variable->variable_name;
    new_variable->params = old_variable->params;
    new_variable->no_of_params = old_variable->no_of_params;
    return new_variable;
}
