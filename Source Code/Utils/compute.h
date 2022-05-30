#ifndef _COMPUTE
#define _COMPUTE

#include "../Datatypes/enums.h"
#include "../Datatypes/structs.h"
#include "typeChecking.h"
#include "typeConversion.h"
#include <math.h>
#include <stdio.h>

// first of all we may have different data types on the right handside
// we will get the highest rank of them
DataTypes compute_highest_rank(DataTypes op1, DataTypes op2)
{
    return (op1>op2)? op1: op2;
}

// compute the result of rhs with setting the type
RETURN_CODES compute_rhs_value(struct lexemeInfo **result, struct lexemeInfo *op1, struct lexemeInfo *op2,Operator operator,int yylineno)
{ 
    if(op2 == NULL && operator == UMINUS_OP){
        if(op1->my_type == INT_DT ||op1->my_type == CONST_INT_DT){
                    set_lexemeInfo(result, INT_DT);
                    (*result)->intValue = - op1->intValue ;
        }else if(op1->my_type == FLOAT_DT || op1->my_type == CONST_FLOAT_DT){
                    set_lexemeInfo(result, FLOAT_DT);
                    (*result)->floatValue = - op1->floatValue ;
        }else{
            return OPERATION_NOT_SUPPORTED;
        }
        return SUCCESS;
    }
    
    DataTypes max = compute_highest_rank(op1->my_type, op2->my_type);
    // check if same datatype
    RETURN_CODES is_equal = check_both_sides(op1->my_type, op2->my_type);
    if(is_equal == FAILURE){
        // incase they are not the same datatype
        // we need first to detect the one with lower datatype and upgrade it
        if(op1->my_type == max){
            // therefore op2 is the one needed to be upgraded
            DataTypes previous_type = op2->my_type;     
            op2->my_type = max;
            upgrade_my_value(&op2, previous_type, max, yylineno);
        }else if(op2->my_type == max){
            // therefore op1 is the one needed to be upgraded
            DataTypes previous_type = op2->my_type;     
            op1->my_type = max;
            upgrade_my_value(&op1, previous_type, max, yylineno);
        }
        is_equal = SUCCESS;
    }

    if (is_equal == SUCCESS)
    {
        if(max < INT_DT){  
            // make result of type int if its < int, ex bool or char
            set_lexemeInfo(result, INT_DT);
            if(max == BOOL_DT || max == CONST_BOOL_DT){
                // check first if we needed to upgrade a certain operand
                if(operator == PLUS_OP){
                    (*result)->intValue = (int)(op1->boolValue + op2->boolValue);
                }else if(operator == MINUS_OP){
                    (*result)->intValue = (int)(op1->boolValue - op2->boolValue);
                }else{
                    return OPERATION_NOT_SUPPORTED;
                }
                printf("implicit conversion from bool to int in line number: %d\n",yylineno);
            }
            else if(max == CHAR_DT || max == CONST_CHAR_DT){// if its char
                if(operator == PLUS_OP){
                    (*result)->intValue = (int)(op1->charValue + op2->charValue); 
                }else if(operator == MINUS_OP){
                    (*result)->intValue = (int)(op1->charValue - op2->charValue);
                }else{
                    return OPERATION_NOT_SUPPORTED;
                }
                printf("implicit conversion from char to int in line number: %d\n",yylineno);
            }
        }else{
            // data type is int or float
            if(max == INT_DT || max == CONST_INT_DT){
                set_lexemeInfo(result, max);
                if(operator == PLUS_OP){
                    (*result)->intValue = op1->intValue + op2->intValue;
                }else if(operator == MINUS_OP){
                    (*result)->intValue = op1->intValue - op2->intValue;
                }else if(operator == DIVIDE_OP){
                    if(op2->intValue == 0){
                        return DIVISION_BY_ZERO_ERROR;
                    }
                    (*result)->intValue = op1->intValue / op2->intValue;
                }else if(operator == MULTIPLY_OP){
                    (*result)->intValue = op1->intValue * op2->intValue;
                }else if(operator == REM_OP){
                    (*result)->intValue = op1->intValue % op2->intValue;
                }else if(operator == POWER_OP){
                    (*result)->intValue = pow(op1->intValue,op2->intValue);
                }
            }else if(max == FLOAT_DT || max == CONST_FLOAT_DT){
                set_lexemeInfo(result, max);
                if(operator == PLUS_OP){
                    (*result)->floatValue = op1->floatValue + op2->floatValue;
                }else if(operator == MINUS_OP){
                    (*result)->floatValue = op1->floatValue - op2->floatValue;
                }else if(operator == DIVIDE_OP){
                    if(op2->floatValue == 0){
                        return DIVISION_BY_ZERO_ERROR;
                    }
                    (*result)->floatValue = op1->floatValue / op2->floatValue;
                }else if(operator == MULTIPLY_OP){
                    (*result)->floatValue = op1->floatValue * op2->floatValue;
                }else if(operator == REM_OP){
                    return OPERATION_NOT_SUPPORTED;
                }else if(operator == POWER_OP){
                    (*result)->floatValue =  pow(op1->floatValue,op2->floatValue);;
                }
            }else if(max == STRING_DT || max == CONST_STRING_DT){
                return STRING_INVALID_OPERATION;
            }
        }
        return SUCCESS; // no operations to do
    }
}
#endif