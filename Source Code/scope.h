#include "symbolTable.h"

struct scope{
    symbolTable* my_table ; // current table in this scope
    scope* my_parent = NULL ; // ptr to my parent scope
    
    struct variable_entry* find_variable(char* variable_to_find) {
        variable_entry* variable_to_find = NULL;
        
        variable_to_find = my_table->find_variable(variable_to_find);
        if(variable_to_find != NULL ){
            return variable_to_find ;
        }
        scope* upper_scopes = my_parent ;
        while(upper_scopes != NULL){
            upper_scopes->my_table->find_variable(variable_to_find);
            if(variable_to_find != NULL ){
                return variable_to_find ;
            }else{
                // go up one more level
                upper_scopes = upper_scopes->my_parent;
            }
        }
        return NULL;
    }

    void add_variable_to_symbolTable(char* name, int is_init,DataTypes datatype ) {
        // making a new variable 
        struct variable_entry* new_variable = (struct variable_entry*)malloc(sizeof(struct variable_entry));
        
        // setting the variable info
        new_variable->is_initialized = is_init;
        new_variable->is_used = 0 ;
        new_variable->my_datatype = datatype;
        new_variable->variable_name = name;
        my_table->add_variable_to_symbolTable(new_variable);
    }
};