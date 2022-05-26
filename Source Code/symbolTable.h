#include "uthash.h"

typedef enum{
    FUNCTION, PARAMETER, VARIABLE
} Kind;

typedef enum{
    SUCCESS, FAILURE
} RETURN_CODES;

typedef enum{
    INT, FLOAT, CHAR, STRING, Const_INT, Const_FLOAT, Const_CHAR, Const_STRING , function_Datatype
} DataTypes;

struct function_Datatype{
    DataTypes* inputs; // array of inputs
    DataTypes* output; // array of outputs
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
    struct variable_entry* table;
};


struct variable_entry* find_variable(struct symbolTable *symbolTable, char* variable_to_find) {
    
    struct variable_entry * variable;
    HASH_FIND_STR(symbolTable->table, variable_to_find, variable );
    return variable;
}

RETURN_CODES add_variable_to_symbolTable(struct symbolTable *symbolTable, struct variable_entry * variable_to_add) {
        
    // first we need to check if it exist before
    if(find_variable(symbolTable, variable_to_add->variable_name) != NULL){
        // therefore multiple definition
        return FAILURE;
    }

    HASH_ADD_STR(symbolTable->table, variable_name, variable_to_add );
    return SUCCESS;
}
    


