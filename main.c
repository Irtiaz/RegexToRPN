#include <stdio.h>
#include <string.h>

#include "stack.h"
#include "stb_ds.h"

#define MAX_TOKEN_LENGTH 5

typedef enum { LEFT, RIGHT } Associativity;

typedef struct {
  const char *symbol;
  Associativity associativity;
} Operator;

void to_reverse_polish_notation(char infix_tokens[][MAX_TOKEN_LENGTH],
                                size_t number_of_tokens, Operator *operators);
int token_to_operator_index(const char *token, Operator *operators);
size_t get_precedence(Operator operator, Operator * operators);

int main(void) {
  Operator *operators = NULL;
  char infix_tokens[][MAX_TOKEN_LENGTH] = {"(", "1", "+", "2", ")", "*", "3"};

  Operator multiply = {"*", LEFT};
  Operator addition = {"+", LEFT};
  Operator subtraction = {"-", LEFT};

  arrput(operators, multiply);
  arrput(operators, addition);
  arrput(operators, subtraction);

  to_reverse_polish_notation(infix_tokens, 7, operators);

  arrfree(operators);
  return 0;
}

void to_reverse_polish_notation(char infix_tokens[][MAX_TOKEN_LENGTH],
                                size_t number_of_tokens, Operator *operators) {
  size_t i;

  Operator *operator_stack = NULL;
  int *open_parenthesis_indices_in_operator_stack = NULL;

  for (i = 0; i < number_of_tokens; ++i) {
    char *token = infix_tokens[i];
    int operator_index = token_to_operator_index(token, operators);

    if (operator_index >= 0) {
      /* Token IS an operator */
      Operator x = operators[operator_index];
      size_t precedence_of_x = get_precedence(x, operators);

      /*
       While there is an operator (y) at the top of the operators stack and
       either (x) is left-associative and its precedence is less or equal to
       that of (y), or (x) is right-associative and its precedence is less than
       (y)
      */

      while (!stack_is_empty(operator_stack)) {
        Operator y = stack_peek(operator_stack);
        size_t precedence_of_y = get_precedence(y, operators);

        int x_is_left_associative = x.associativity == LEFT;
        int x_is_right_associative = x.associativity == RIGHT;
        int x_precedence_is_less_than_y = precedence_of_x < precedence_of_y;
        int x_precedence_is_less_or_equal_to_y =
            precedence_of_x <= precedence_of_y;

        if ((x_is_left_associative && x_precedence_is_less_or_equal_to_y) ||
            (x_is_right_associative && x_precedence_is_less_than_y)) {
          /* pop y from the stack and add it to the output buffer */
          (void)stack_pop(operator_stack);
          printf("%s\n", y.symbol);
        }

        else {
          break;
        }
      }

      /* Push x on the operator stack */
      stack_push(operator_stack, x);
    }

    else if (strlen(token) == 1 && (token[0] == '(' || token[0] == ')')) {
      /* Our token is a parenthesis */
      char parenthesis_token = token[0];
      if (parenthesis_token == '(') {
        /* Open parenthesis encountered, push its position */
        int current_index_of_operator_stack = stack_length(operator_stack);
        stack_push(open_parenthesis_indices_in_operator_stack,
                   current_index_of_operator_stack);
      } else {
        /* Closed parenthesis encountered */
        int last_open_parenthesis_index =
            stack_pop(open_parenthesis_indices_in_operator_stack);
        while (stack_length(operator_stack) > last_open_parenthesis_index) {
          Operator top_of_the_stack = stack_pop(operator_stack);
          printf("%s\n", top_of_the_stack.symbol);
        }
      }
    }

    else {
      /* our token is a regular token
       * just add it to the output buffer
       */
      printf("%s\n", token);
    }
  }

  /* while there are still operators in the stack, pop and add them to the
   * output buffer */
  while (arrlen(operator_stack) != 0) {
    Operator top_of_the_stack = stack_pop(operator_stack);
    printf("%s\n", top_of_the_stack.symbol);
  }

  stack_free(open_parenthesis_indices_in_operator_stack);
  stack_free(operator_stack);
}

int token_to_operator_index(const char *token, Operator *operators) {
  size_t i;

  if (strlen(token) > 1)
    return -1;

  for (i = 0; i < (size_t)arrlen(operators); ++i) {
    if (strcmp(operators[i].symbol, token) == 0)
      return i;
  }

  return -1;
}

size_t get_precedence(Operator operator, Operator * operators) {
  size_t i;
  size_t number_of_operators = arrlen(operators);
  for (i = 0; i < number_of_operators; ++i) {
    if (operators[i].symbol == operator.symbol)
      return number_of_operators - i;
  }

  fprintf(stderr,
          "At get_precedence(): operator %s not found in the operators list",
          operator.symbol);
  exit(1);
}
