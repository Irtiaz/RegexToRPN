#include <stdio.h>
#include <string.h>

#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

#define MAX_TOKEN_LENGTH 5

typedef enum  {
  LEFT,
  RIGHT
} Associativity;

typedef struct {
  char symbol;
  Associativity associativity;
} Operator;

Operator operators[] = {
  {'*', LEFT},
  {'+', LEFT},
  {'-', LEFT},
};

void to_reverse_polish_notation(char infix_tokens[][MAX_TOKEN_LENGTH], size_t number_of_tokens);
int token_to_operator_index(const char *token);
size_t get_precedence(Operator operator);

int main(void) {
  char infix_tokens[][MAX_TOKEN_LENGTH] = {"1", "+", "2", "*", "3"};

  to_reverse_polish_notation(infix_tokens, 5);

  return 0;
}
void to_reverse_polish_notation(char infix_tokens[][MAX_TOKEN_LENGTH], size_t number_of_tokens) {
  size_t i;

  Operator *operator_stack = NULL;

  for (i = 0; i < number_of_tokens; ++i) {
    char *token = infix_tokens[i];
    int operator_index = token_to_operator_index(token);

    if (operator_index >= 0) {
      /* Token IS an operator */
      Operator x = operators[operator_index];
      size_t precedence_of_x = get_precedence(x);

      /*
       While there is an operator (y) at the top of the operators stack and either (x) is left-associative and its precedence is less or equal to that of (y), or (x) is right-associative and its precedence is less than (y)
      */

      while (arrlen(operator_stack) > 0) {
        Operator y = operator_stack[arrlen(operator_stack) - 1];
        size_t precedence_of_y = get_precedence(y);

        int x_is_left_associative = x.associativity == LEFT;
        int x_is_right_associative = x.associativity == RIGHT;
        int x_precedence_is_less_than_y = precedence_of_x < precedence_of_y;
        int x_precedence_is_less_or_equal_to_y = precedence_of_x <= precedence_of_y;
        
        if ((x_is_left_associative && x_precedence_is_less_or_equal_to_y) || (x_is_right_associative && x_precedence_is_less_than_y)) {
          /* pop y from the stack and add it to the output buffer */
          arrdel(operator_stack, arrlen(operator_stack) - 1);
          printf("%c\n", y.symbol);
        }

        else break;
      }

      /* Push x on the operator stack */
      arrput(operator_stack, x);
    }

    else {
      /* our token is a regular token
       * just add it to the output buffer
      */
      printf("%s\n", token);
    }

  }

  /* while there are still operators in the stack, pop and add them to the output buffer */
  while (arrlen(operator_stack) != 0) {
    Operator top_of_the_stack = operator_stack[arrlen(operator_stack) - 1];
    printf("%c\n", top_of_the_stack.symbol);
    arrdel(operator_stack, arrlen(operator_stack) - 1);
  }

  arrfree(operator_stack);
}

int token_to_operator_index(const char *token) {
  size_t i;
  
  if (strlen(token) > 1) return -1;

  for (i = 0; i < sizeof(operators) / sizeof(Operator); ++i) {
    if (operators[i].symbol == token[0]) return i;
  }

  return -1;
}

size_t get_precedence(Operator operator) {
  size_t i;
  size_t number_of_operators = sizeof(operators) / sizeof(Operator);
  for (i = 0; i < number_of_operators; ++i) {
    if (operators[i].symbol == operator.symbol) return number_of_operators - i;
  }
  
  fprintf(stderr, "At get_precedence(): operator %c not found in the operators list", operator.symbol);
  exit(1);
}
