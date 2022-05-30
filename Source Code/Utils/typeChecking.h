#ifndef _TYPECHECKING
#define _TYPECHECKING
#include "../Datatypes/enums.h"
#include "typeConversion.h""
#include "compute.h"


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

#endif