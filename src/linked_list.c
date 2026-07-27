#include "linked_list.h"
#include "node.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct list {
    node* head;
    node* tail;
    size_t len;
    free_func destroy;
    cmp_func cmp;
    print_func print;
} list;

list* new_list(free_func destroy, cmp_func cmp, print_func print) {
    if (cmp == NULL) {
        return NULL;
    }

    list* l = malloc(sizeof(*l));

    if (l == NULL) {
        return NULL;
    }

    l->head = NULL;
    l->tail = NULL;
    l->len = 0;
    l->destroy = destroy;
    l->cmp = cmp;
    l->print = print;

    return l;
}

status list_add_first(list* l, void* value) {
    if (l == NULL) {
        return NULL_POINTER;
    }

    node* n = new_node(value);

    if (n == NULL) {
        return ALLOC_FAIL;
    }

    if (l->len == 0) {
        l->head = l->tail = n;
    }

    else {
        n->next = l->head;
        l->head = n;
    }

    l->len++;

    return SUCCESS;
}

status list_add_last(list* l, void* value) {
    if (l == NULL) {
        return NULL_POINTER;
    }

    node* n = new_node(value);

    if (n == NULL) {
        return ALLOC_FAIL;
    }

    if (l->len == 0) {
        l->head = l->tail = n;
    }

    else {
        l->tail->next = n;
        l->tail = n;
    }

    l->len++;

    return SUCCESS;
}

status list_add(list* l, void* value, size_t index) {
    if (l == NULL) {
        return NULL_POINTER;
    }

    if (index > l->len) {
        return OUT_OF_RANGE;
    }

    if (index == 0) {
        return list_add_first(l, value);
    }

    if (index == l->len) {
        return list_add_last(l, value);
    }

    node* n = new_node(value);

    if (n == NULL) {
        return ALLOC_FAIL;
    }

    node* current = l->head;

    for (size_t i = 0; i < index - 1; i++) {
        current = current->next;
    }

    n->next = current->next;
    current->next = n;
    l->len++;

    return SUCCESS;
}

static void list_remove_node(list* l, node* current, node* previous) {
    if (current == l->head) {
        l->head = current->next;
    } else {
        previous->next = current->next;
    }

    if (current == l->tail) { 
        l->tail = previous;
    }
}

status list_remove_value(list* l, const void* value) {
    if (l == NULL) {
        return NULL_POINTER;
    }

    if (l->len == 0) {
        return UNDERFLOW;
    }

    node* current = l->head;
    node* previous = NULL;

    while (current != NULL) {
        if (l->cmp(current->value, value) == 0) {
            list_remove_node(l, current, previous);

            if (l->destroy != NULL && current->value != NULL) {
                l->destroy(current->value);
            }

            free(current);
            l->len--;

            return SUCCESS;
        }

        previous = current;
        current = current->next;
    }

    return NOT_FOUND;
}

status list_remove_index(list* l, size_t index) {
    if (l == NULL) {
        return NULL_POINTER;
    }

    if (l->len == 0) {
        return UNDERFLOW;
    }

    if (index >= l->len) {
        return OUT_OF_RANGE;
    }

    node* current = l->head;
    node* previous = NULL;

    for (size_t i = 0; i < index; i++) {
        previous = current;
        current = current->next;
    }   

    list_remove_node(l, current, previous);

    if (l->destroy != NULL && current->value != NULL) {
        l->destroy(current->value);
    }

    free(current);
    l->len--;

    return SUCCESS;
}

void* list_get(const list* l, size_t index) {
    if (l == NULL || index >= l->len) {
        return NULL;
    }

    node* current = l->head;

    for (size_t i = 0; i < index; i++) {
        current = current->next;
    }

    return current->value;
}

bool list_contains(const list* l, const void* value) {
    if (l == NULL) return false;

    node* current = l->head;

    while (current != NULL) {
        if (l->cmp(current->value, value) == 0) {
            return true;
        }
        current = current->next;
    }

    return false;
}

bool list_is_empty(const list* l) {
    if (l == NULL) return true;
    return l->len == 0;
}

size_t list_size(const list* l) {
    if (l == NULL) return 0;
    return l->len;
}

void list_clear(list* l) {
    if (l == NULL) return;

    node* current = l->head;

    while (current != NULL) {
        node* next = current->next;

        if (l->destroy != NULL && current->value != NULL) {
            l->destroy(current->value);
        }

        free(current);
        current = next;
    }

    l->head = NULL;
    l->tail = NULL;
    l->len = 0;
}

void list_print(const list* l) {
    if (l == NULL || l->len == 0 || l->print == NULL) {
        return;
    }

    node* current = l->head;

    while (current != NULL) {
        l->print(current->value);
        if (current->next != NULL) printf(" -> ");
        current = current->next;
    }
}

void list_free(list* l) {
    if (l == NULL) return;
    list_clear(l);
    free(l);
}