#include "symbolTable.h"

struct scope{
    struct symbolTable* my_table ; // current table in this scope
    struct scope* my_parent ; // ptr to my parent scope
};

struct variable_entry* find_variable_in_scope(struct scope* my_scope, char* variable_to_find) {
        struct variable_entry* variable_found = NULL;
        
        variable_found =  find_variable(my_scope->my_table, variable_to_find);

        if(variable_found != NULL ){
            return variable_found ;
        }

        struct scope* upper_scopes = my_scope->my_parent ;
        while(upper_scopes != NULL){
            variable_found =  find_variable(upper_scopes->my_table, variable_to_find);
            if(variable_found != NULL ){
                return variable_found ;
            }else{
                // go up one more level
                upper_scopes = upper_scopes->my_parent;
            }
        }
        return NULL;
    }

    RETURN_CODES add_variable_to_scope(struct scope* my_scope, char* name, int is_init,DataTypes datatype ) {
        // making a new variable 
        struct variable_entry* new_variable = (struct variable_entry*)malloc(sizeof(struct variable_entry));
        
        // setting the variable info
        new_variable->is_initialized = is_init;
        new_variable->is_used = 0 ;
        new_variable->my_datatype = datatype;
        new_variable->variable_name = name;
        return add_variable_to_symbolTable(my_scope->my_table,new_variable);
    }