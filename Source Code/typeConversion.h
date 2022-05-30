#pragma once
#include "Datatypes/enums.h"
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
RETURN_CODES downgrade_my_value(struct lexemeInfo **op, DataTypes prevdt, DataTypes newdt, int yylineno){
    
    if(newdt == CHAR_DT || newdt == CONST_CHAR_DT){
        if(prevdt == INT_DT || prevdt == CONST_INT_DT){
            (*op)->charValue = (char)(*op)->intValue ;
            printf("implicit conversion from int to char in line number: %d\n",yylineno);
        }
        else if(prevdt == FLOAT_DT || prevdt == CONST_FLOAT_DT){
            (*op)->charValue = (char)(*op)->floatValue ;
            printf("implicit conversion from float to char in line number: %d\n",yylineno);
        }
    }else if(newdt == INT_DT || newdt == CONST_INT_DT){
        if(prevdt == FLOAT_DT || prevdt == CONST_FLOAT_DT){
            (*op)->intValue = (int)(*op)->floatValue ;
            printf("implicit conversion from float to int in line number: %d\n",yylineno);
        }
    }else if(newdt == BOOL_DT || newdt == CONST_BOOL_DT){
        if(prevdt == CHAR_DT || prevdt == CONST_CHAR_DT){
            (*op)->boolValue = (int)(*op)->charValue != 0 ;
            printf("implicit conversion from char to bool in line number: %d\n",yylineno);
        }
        else if(prevdt == INT_DT || prevdt == CONST_INT_DT){
            (*op)->boolValue = (int)(*op)->intValue != 0 ;
            printf("implicit conversion from int to bool in line number: %d\n",yylineno);
        }
        else if(prevdt == FLOAT_DT || prevdt == CONST_FLOAT_DT){
            (*op)->boolValue = (int)(*op)->floatValue != 0 ;
            printf("implicit conversion from float to bool in line number: %d\n",yylineno);
        }
    }else{
        return STRING_INVALID_OPERATION;
    }
    return SUCCESS;
}

RETURN_CODES upgrade_my_value(struct lexemeInfo **op, DataTypes prevdt, DataTypes newdt,int yylineno){
    if(newdt == CHAR_DT || newdt == CONST_CHAR_DT){
        // since new is character so we knew that old must have been a boolean
        (*op)->charValue = (char)(*op)->boolValue;
        printf("implicit conversion from bool to char in line number: %d\n",yylineno);
    }else if(newdt == INT_DT || newdt == CONST_INT_DT){
        // since new is int so we knew that old must have been a boolean or a character
        if(prevdt == BOOL_DT || prevdt == CONST_BOOL_DT){
            (*op)->intValue = (int)(*op)->boolValue;
            printf("implicit conversion from bool to int in line number: %d\n",yylineno);
        }
        else if(prevdt == CHAR_DT || prevdt == CONST_CHAR_DT){
            (*op)->intValue = (int)((*op)->charValue);
            printf("implicit conversion from char to int in line number: %d\n",yylineno);
        }
    }else if(newdt == FLOAT_DT || newdt == CONST_FLOAT_DT){
        // since new is int so we knew that old must have been a boolean or a character or an int
        if(prevdt == BOOL_DT || prevdt == CONST_BOOL_DT){
            (*op)->floatValue = (float)(*op)->boolValue;
            printf("implicit conversion from bool to float in line number: %d\n",yylineno);
        }
        else if(prevdt == CHAR_DT || prevdt == CONST_CHAR_DT){
            (*op)->floatValue = (float)(*op)->charValue;
            printf("implicit conversion from char to float in line number: %d\n",yylineno);
        }
        else if(prevdt == INT_DT || prevdt == CONST_INT_DT){
            (*op)->floatValue = (float)(*op)->intValue;
            printf("implicit conversion from int to float in line number: %d\n",yylineno);
        }
    }else{
        return STRING_INVALID_OPERATION;
    }
    return SUCCESS;
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


RETURN_CODES down_convert_boolean_expression(struct lexemeInfo **result, struct lexemeInfo *expr1, struct lexemeInfo *expr2,Boolean_Operator operator){

    if(expr1->my_type == STRING_DT || expr1->my_type == CONST_STRING_DT || expr2->my_type == STRING_DT || expr2->my_type == CONST_STRING_DT) 
    {
        // since strings cant be downconverted to booleans
        return STRING_INVALID_OPERATION;
    }
    set_lexemeInfo(result , BOOL_DT);
    // TODO:: compute real result ??
    (*result)->boolValue = 0;
    return SUCCESS;
}

OperationsToDo implicit_conversion(DataTypes lhs, DataTypes rhs)
{
    if(
        ((lhs!= STRING_DT && lhs!= CONST_STRING_DT ) && (rhs == STRING_DT || rhs == CONST_STRING_DT))
        ||
        ((rhs!= STRING_DT && rhs!= CONST_STRING_DT ) && (lhs == STRING_DT || lhs == CONST_STRING_DT))
    ){
        return RAISE_ERROR;
    }
    
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
