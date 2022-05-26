#include "uthash.h"

enum Kind{
    FUNCTION, PARAMETER, VARIABLE
};

enum RETURN_CODES{
    SUCCESS, FAILURE
};

struct function_Datatype{
    DataTypes* inputs; // array of inputs
    DataTypes* output; // array of outputs
};

enum DataTypes{
    INT, FLOAT, CHAR, STRING, Const_INT, Const_FLOAT, Const_CHAR, Const_STRING , function_Datatype
};

struct variable_entry {
    char* variable_name; // we will use the name of variable as key
    DataTypes my_datatype;
    int is_initialized ; // 0 : uninit , 1 : init
    
    // for unused variables we will give warnings bec its not used through the whole program
    int is_used ; // 0 : unused , 1 : used
    
    UT_hash_handle hh; /* makes this structure hashable */
};

struct symbolTable{ 
    struct variable_entry *table = NULL;

    RETURN_CODES add_variable_to_symbolTable(struct variable_entry * variable) {
        
        // first we need to check if it exist before
        if(find_variable(variable->variable_name) != NULL){
            // therefore multiple definition
            return FAILURE;
        }

        HASH_ADD_STR( table, variable_name, variable );
        return SUCCESS;
    }
    
    struct variable_entry* find_variable(char* variable_to_find) {
        
        variable_entry * variable;
        HASH_FIND_STR( table, variable_to_find, variable );
        return variable;
    }

    

};


