#ifndef _TYPECONVERSION
#define _TYPECONVERSION
#include "../Data Structures/enums.h"
#include "../Data Structures/structs.h"
#include <stdio.h>

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

#endif