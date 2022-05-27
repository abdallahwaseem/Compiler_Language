typedef enum
{
    FUNCTION,
    PARAMETER,
    VARIABLE
} Kind;

typedef enum
{
    SUCCESS,
    FAILURE,
    CONSTANT_NOT_INITIALIZED,
    CONSTANT_REASSIGNMENT
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
    STRING_DT,
    CONST_STRING_DT,
    VOID_DT
    // last ones are the highest in rank
} DataTypes;

struct lexemeInfo
{
    int intValue;
    float floatValue;
    char charValue;
    char *stringValue;
    int boolValue;
    DataTypes my_type;
};

void set_lexemeInfo(struct lexemeInfo **input_lexeme, DataTypes my_type)
{
    (*input_lexeme) = (struct lexemeInfo *)malloc(sizeof(struct lexemeInfo));
    (*input_lexeme)->my_type = my_type;
}

// linked list of arguments
struct argument_info
{
    DataTypes my_type;
    char *my_name;
    struct argument_info *next_arg; // points to next arguments in linked list
};
