#include "symbolTable.h"

struct converter
{
    DataTypes *inputs; // array of inputs in right hand side
    DataTypes output;  // the output datatype; not array since we have one output
};

typedef enum
{
    NONE,
    EVAL_THEN_DOWNGRADE_RHS,
    EVAL_THEN_UPGRADE_RHS
} OperationsToDo;

struct converter initialize_converter(DataTypes *in_inputs, DataTypes in_outputs)
{
    struct converter my_converter;
    my_converter.inputs = in_inputs;
    my_converter.output = in_outputs;
    return my_converter;
}

RETURN_CODES check_both_sides(struct converter *my_converter)
{
    for (size_t i = 0; i < sizeof(my_converter->inputs) / sizeof(my_converter->inputs[0]); i++)
    {
        if (my_converter->inputs[i] == my_converter->output)
            return FAILURE;
    }
    return SUCCESS;
}

OperationsToDo implicit_conversion(struct converter *my_converter)
{
    // The difference in rank decides the promotion and demotion of the right expression
    // to equalize the rank of left and right expression.

    // first of all check promotion:
    // Promotion does not create any problems.
    // The rank of right expression is promoted to the rank of left expression.
    // The value of the expression is the value of the right expression after the promotion.

    // first of all we may have different data types on the right handside
    // we will get the highest rank of them
    DataTypes highest_rank;
    for (size_t i = 0; i < sizeof(my_converter->inputs) / sizeof(my_converter->inputs[0]); i++)
    {
        highest_rank = __max(highest_rank, my_converter->inputs[i]);
    }

    // then we need to promote lower ranks to that rank
    for (size_t i = 0; i < sizeof(my_converter->inputs) / sizeof(my_converter->inputs[0]); i++)
    {
        my_converter->inputs[i] = highest_rank;
    }

    // now we know rank of the right hand side of the assignment then we have 2 cases :
    // Promoting implies that the right expression is of lower rank.
    // Demoting implies that the right expression is of higher rank.
    if (highest_rank <= my_converter->output)
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
    else
    {
        // so output rank is lower, that may result in errors
        // bool  b = false;
        // char  c = ‘X’;
        // b = c;     // value of b is 1 (true)
        // so we casted the right hand side to the upper datatype
        // TODO VERY IMPORTANT
        // we must evaluate first ex: bool b = 2 - 1;
        // the result = bool(2-1) = bool(1) = 1
        // not bool(2) - bool(1) = 1 - 1 = 0
        return EVAL_THEN_DOWNGRADE_RHS;
    }
}

OperationsToDo checkAssignment(struct converter *my_converter)
{

    // check if same datatype
    RETURN_CODES is_equal = check_both_sides(my_converter);
    if (is_equal == SUCCESS)
        return NONE; // no operations to do

    // if not same datatype we need to check the implicit conversion
    return implicit_conversion(my_converter);
}
