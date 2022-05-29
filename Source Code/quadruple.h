#include "dataTypes.h"
#include <string.h>
#include <stdio.h>

struct quadruple_info
{
  char *opcode;
  char *operand1;
  char *operand2;
  int result;
};
// define stack
struct Stack
{
  int top;
  char* items[100];
  int id_quadruple; // t1,t2 
};

char* pop(struct Stack **quad_stack){
  return (*quad_stack)->items[--((*quad_stack)->top)];
}

void push(struct Stack *quad_stack, char *str)
{
  quad_stack->items[quad_stack->top] = (char *)malloc(strlen(str) + 1);
  strcpy(quad_stack->items[quad_stack->top], str);
  quad_stack->top++;
  if(quad_stack->top == 3){
    // pop stack and print 
    char temp[4]; 
    itoa(quad_stack->id_quadruple,temp,10);
    struct quadruple_info* quad_info = (struct quadruple_info *)malloc(sizeof(struct quadruple_info));
    quad_info->opcode = pop(&quad_stack);
    quad_info->operand2 = pop(&quad_stack);
    quad_info->operand1 = pop(&quad_stack);
    quad_info->result = quad_stack->id_quadruple++;
    char destination[] = "t";
    strcat(destination,temp);
    printf("%s %s %s %s \n",destination,quad_info->operand1,quad_info->opcode,quad_info->operand2);
    push(quad_stack,destination);//pushing our destination
  }

}
void print_stack(struct Stack *quad_stack)
{
  // print stack
  for (int i = 1; i <= quad_stack->top; i++)
  {
    printf("%s\n", quad_stack->items[i]);
  }
}