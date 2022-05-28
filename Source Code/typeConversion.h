#pragma once
#include "dataTypes.h"
#include <math.h>


RETURN_CODES check_both_sides(DataTypes in_inputs, DataTypes in_outputs)
{
    if (in_inputs != in_outputs)
        return FAILURE;
    return SUCCESS;
}

// first of all we may have different data types on the right handside
// we will get the highest rank of them
DataTypes compute_highest_rank(DataTypes op1, DataTypes op2)
{
    return (op1>op2)? op1: op2;
}



// compute the result of rhs with setting the type
RETURN_CODES compute_rhs_value(struct lexemeInfo **result, struct lexemeInfo *op1, struct lexemeInfo *op2,Operator operator)
{
    if(op2 == NULL && operator == UMINUS_OP ){
        if(op1->my_type == INT_DT ||op1->my_type == CONST_INT_DT){
                    (*result)->intValue = - op1->intValue ;
        }else if(op1->my_type == FLOAT_DT || op1->my_type == CONST_FLOAT_DT){
                    (*result)->floatValue = - op1->floatValue ;
        }else{
            return OPERATION_NOT_SUPPORTED;
        }
        return SUCCESS;
    }
    
    DataTypes max = compute_highest_rank(op1->my_type, op2->my_type);
    // check if same datatype
    RETURN_CODES is_equal = check_both_sides(op1->my_type, op2->my_type);

    if (is_equal == SUCCESS)
    {
        if(max < INT_DT){  
            // make result of type int if its < int, ex bool or char
            set_lexemeInfo(result, INT_DT);
            if(max == BOOL_DT || max == CONST_BOOL_DT){
                if(operator == PLUS_OP){
                    (*result)->intValue = (int)(op1->boolValue + op2->boolValue);
                }else if(operator == MINUS_OP){
                    (*result)->intValue = (int)(op1->boolValue - op2->boolValue);
                }else{
                    return OPERATION_NOT_SUPPORTED;
                }
            }
            else if(max == CHAR_DT || max == CONST_CHAR_DT)// if its char
                if(operator == PLUS_OP){
                    (*result)->intValue = (int)(op1->charValue + op2->charValue); 
                }else if(operator == MINUS_OP){
                    (*result)->intValue = (int)(op1->charValue - op2->charValue);
                }else{
                    return OPERATION_NOT_SUPPORTED;
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
    // TODO::
    // incase they are not the same datatype
    // switch (max)
    // {
    // case BOOL_DT:

    //     break;

    // default:
    //     break;
    // }
}

OperationsToDo implicit_conversion(DataTypes lhs, DataTypes rhs)
{
    // we assumed that we made the got the max rhs rank

    // The difference in rank decides the promotion and demotion of the right expression
    // to equalize the rank of left and right expression.

    // first of all check promotion:
    // Promotion does not create any problems.
    // The rank of right expression is promoted to the rank of left expression.
    // The value of the expression is the value of the right expression after the promotion.

    // then we need to promote lower ranks to that rank
    // bool b  = (int c) + (char d)

    // new ranks
    // bool b  = (int c) + (int d)

    // now we know rank of the right hand side of the assignment then we have 2 cases :
    // Promoting implies that the right expression is of lower rank.
    // Demoting implies that the right expression is of higher rank.
    if (lhs > rhs)
    {
        // so output rank is higher, so no problem in that
        // example :
        // char        y = ‘X’;
        // int         i = 123;
        // i = y;   // value of i is 88
        // here type of output > type of input so we just cast input to output.
        // TODO VERY IMPORTANT
        // bool  b = 'y'-'x';
        // b here equals true since we made ('y' - 'x') as a whole the convert it to character;
        return EVAL_THEN_UPGRADE_RHS;
    }
    else if (lhs < rhs)
    {
        // so output rank is lower, that may result in errors
        // bool  b = false;
        // char  c = ‘X’;
        // b = c;     // value of b is 1 (true)
        // so we casted the right hand side to the upper datatype
        // TODO VERY IMPORTANT
        // we must evaluate first
        // ex: bool b = 2 - 1; // this should evaluate to true
        // the result = bool(2-1) = bool(1) = 1
        // not bool(2) - bool(1) = 1 - 1 = 0
        return EVAL_THEN_DOWNGRADE_RHS;
    }
    else
    {
        return NONE;
    }
}

OperationsToDo checkAssignment(DataTypes lhs, DataTypes rhs)
{

    // check if same datatype
    RETURN_CODES is_equal = check_both_sides(lhs, rhs);
    if (is_equal == SUCCESS)
        return NONE; // no operations to do

    // if not same datatype we need to check the implicit conversion
    return implicit_conversion(lhs, rhs);
}
