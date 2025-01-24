#ifndef STACK_H_435435
#define STACK_H_435435

#define stack_push(stack, element) arrput((stack), (element))
#define stack_pop(stack) arrpop((stack))
#define stack_peek(stack) arrlast(stack)
#define stack_length(stack) arrlen(stack)
#define stack_is_empty(stack) (arrlen(stack) == 0)
#define stack_free(stack) arrfree(stack)

#endif
