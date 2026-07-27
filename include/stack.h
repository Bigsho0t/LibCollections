#pragma once

#include "types.h"

typedef void (*free_func)(void*);
typedef void (*print_func)(const void*);

typedef struct stack stack;

stack* new_stack(free_func destroy, print_func print);
status stack_push(stack* s, void* value);
void* stack_pop(stack* s);
const void* stack_peek(const stack* s);
bool stack_is_empty(const stack* s);
size_t stack_size(const stack* s);
void stack_clear(stack* s);
void stack_print(const stack* s);
void stack_free(stack* s);