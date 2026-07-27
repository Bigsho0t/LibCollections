#include "stack.h"
#include "node.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct stack {
    node* top;
    size_t len;
    free_func destroy;
    print_func print;
} stack;

stack* new_stack(free_func destroy, print_func print) {
    stack* s = malloc(sizeof(*s));

    if (s == NULL) {
        return NULL;
    }

    s->top = NULL;
    s->len = 0;
    s->print = print;
    s->destroy = destroy;

    return s;
}

status stack_push(stack* s, void* value) {
    if (s == NULL) {
        return NULL_POINTER;
    }

    node* new_top = new_node(value);

    if (new_top == NULL) {
        return ALLOC_FAIL;
    }

    new_top->next = s->top;
    s->top = new_top;
    s->len++;

    return SUCCESS;
}

void* stack_pop(stack* s) {
    if (s == NULL || s->len == 0) {
        return NULL;
    }

    void* value = s->top->value;
    node* temp = s->top;
    s->top = s->top->next;
    free(temp);
    s->len--;

    return value;
}

const void* stack_peek(const stack* s) {
    if (s == NULL || s->len == 0) {
        return NULL;
    }

    return s->top->value;
}

bool stack_is_empty(const stack* s) {
    if (s == NULL) return true;
    return s->len == 0;
}

size_t stack_size(const stack* s) {
    if (s == NULL) return 0;
    return s->len;
}

void stack_clear(stack* s) {
    if (s == NULL) return;

    node* current = s->top;

    while (current != NULL) {
        node* next = current->next;
        if (s->destroy != NULL && current->value != NULL) {
            s->destroy(current->value);
        }
        free(current);
        current = next;
    }

    s->top = NULL;
    s->len = 0;
}

void stack_print(const stack* s) {
    if (s == NULL || s->len == 0 || s->print == NULL) {
        return;
    }

    node* current = s->top;

    while (current != NULL) {
        s->print(current->value);
        if (current->next != NULL) printf("\n|\nv\n");
        current = current->next;
    }
}

void stack_free(stack* s) {
    if (s == NULL) return;
    stack_clear(s);
    free(s);
}