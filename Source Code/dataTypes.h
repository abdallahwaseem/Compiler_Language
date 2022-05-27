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
    Const_BOOL_DT,
    CHAR_DT,
    Const_CHAR_DT,
    INT_DT,
    Const_INT_DT,
    FLOAT_DT,
    Const_FLOAT_DT,
    STRING_DT,
    Const_STRING_DT,
    function_Datatype_DT
    // last ones are the highest in rank
} DataTypes;


struct lexemeInfo{
    int intValue;
    float floatValue;
	char charValue;                
    char* stringValue;
	int boolValue;
    DataTypes my_type;
};