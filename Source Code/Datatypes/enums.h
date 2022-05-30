#ifndef _ENUMS
#define _ENUMS


typedef enum
{
    FUNCTION_KIND,
    PARAMETER_KIND,
    VARIABLE_KIND
} Kind;

typedef enum
{
    SUCCESS,
    FAILURE,
    CONSTANT_NOT_INITIALIZED,
    CONSTANT_REASSIGNMENT,
    STRING_INVALID_OPERATION,
    OPERATION_NOT_SUPPORTED,
    DIVISION_BY_ZERO_ERROR
} RETURN_CODES;

typedef enum
{
    // first ones are the lowest in rank
    // rank is: BOOL,CHAR,INT,FLOAT
    // so i made bool and const bool of same rank and so on
    // i ranked them here because we will use that in implicit type conversion
    BOOL_DT,
    CONST_BOOL_DT,
    CHAR_DT,
    CONST_CHAR_DT,
    INT_DT,
    CONST_INT_DT,
    FLOAT_DT,
    CONST_FLOAT_DT,
    // last ones are the highest in rank
    STRING_DT,
    CONST_STRING_DT,
    VOID_DT
} DataTypes;

typedef enum
{
    NONE,
    EVAL_THEN_DOWNGRADE_RHS,
    EVAL_THEN_UPGRADE_RHS,
    RAISE_ERROR
} OperationsToDo;

typedef enum
{
    PLUS_OP,
    MINUS_OP,
    DIVIDE_OP,
    MULTIPLY_OP,
    REM_OP,
    POWER_OP,
    UMINUS_OP
} Operator;

typedef enum
{
    AND_OP,
    OR_OP,
    NOT_OP,
    GREATERTHAN_OP,
    GREATERTHANOREQUAL_OP,
    LESSTHAN_OP,
    LESSTHANOREQUAL_OP,
    EQUALEQUAL_OP,
    NOTEQUAL_OP
} Boolean_Operator;

#endif