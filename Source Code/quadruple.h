#include "dataTypes.h"
#include "string.h"
#include <stdio.h>

struct quadruple_info
{
  char *opcode;
  char *operand1;
  char *operand2;
  char *result;
};
// define stack
struct Stack
{
  int top;
  char *items[100];
};

void push(struct Stack *quad_stack, char *str)
{
  quad_stack->top++;
  quad_stack->items[quad_stack->top] = (char *)malloc(strlen(str) + 1);
  strcpy(quad_stack->items[quad_stack->top], str);
}
void print_stack(struct Stack *quad_stack)
{
  // print stack
  for (int i = 1; i <= quad_stack->top; i++)
  {
    printf("%s\n", quad_stack->items[i]);
  }
}