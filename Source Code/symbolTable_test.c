#include "scope.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
    
    struct scope mainScope = initialize_scope();

    RETURN_CODES op1 = add_variable_to_scope(&mainScope, "x", 1,INT);
    if(op1 == SUCCESS){
        printf("added successfully\n");
    }
    
    struct variable_entry * found = find_variable_in_scope(&mainScope , "x");
    if(found != NULL){
        printf("found !!!\n");
        printf("%s %d\n", found->variable_name , found->is_initialized);
    }else{
        printf("bug in testcase 1\n");
    }
    op1 = add_variable_to_scope(&mainScope, "x", 1,INT);
    if(op1 == FAILURE){
        printf("failed to add!!!\n");
    }else{
        printf("bug in testcase 2\n");
    }
    //////////////////////////////////
    // trying parent scope
    struct scope childScope = initialize_scope();
    set_parent_of_scope(&childScope,&mainScope);
    struct variable_entry * found2 = find_variable_in_scope(&childScope , "x");
    if(found2 != NULL){
        printf("founded in my parent !!!\n");
        printf("%s %d\n", found2->variable_name , found2->is_initialized);
    }else{
        printf("bug in testcase 3\n");
    }
    
    /////////////////////
    // trying parent scope
    struct scope childScope2 = initialize_scope();
    struct variable_entry * found3 = find_variable_in_scope(&childScope2 , "x");
    if(found3 == NULL){
        printf("not founded since i have no parent !!!\n");
    }else{
        printf("bug in testcase 4\n");
    }
    ////////////////////
    // multi level parent
    struct scope childScope3 = initialize_scope();
    set_parent_of_scope(&childScope3,&childScope);
    struct variable_entry * found4 = find_variable_in_scope(&childScope3 , "x");
    if(found4 != NULL){
        printf("founded in my grand parent !!!\n");
        printf("%s %d\n", found4->variable_name , found4->is_initialized);
    }else{
        printf("bug in testcase 5\n");
    }

}
