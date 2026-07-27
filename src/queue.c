#include "queue.h"
#include "node.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct queue {
    node* first;
    node* last;
    size_t len;
    free_func destroy;
    print_func print;
} queue;

queue* new_queue(free_func destroy, print_func print) {
    queue* q = malloc(sizeof(*q));

    if (q == NULL) {
        return NULL;
    }

    q->first = NULL;
    q->last = NULL;
    q->len = 0;
    q->destroy = destroy;
    q->print = print;

    return q;
}

status queue_enqueue(queue* q, void* value) {
    if (q == NULL) {
        return NULL_POINTER;
    }

    node* new_last = new_node(value);

    if (new_last == NULL) {
        return ALLOC_FAIL;
    }

    if (q->len == 0) {
        q->first = q->last = new_last;
    }

    else {
        q->last->next = new_last;
        q->last = new_last;
    }

    q->len++;

    return SUCCESS;
}

void* queue_dequeue(queue* q) {
    if (q == NULL || q->len == 0) {
        return NULL;
    }

    void* value = q->first->value;
    node* temp = q->first;
    q->first = q->first->next;

    if (q->first == NULL) {
        q->last = NULL;
    }

    free(temp);
    q->len--;

    return value;
}

const void* queue_front(const queue* q) {
    if (q == NULL || q->len == 0) {
        return NULL;
    }

    return q->first->value;
}

bool queue_is_empty(const queue* q) {
    if (q == NULL) return true;
    return q->len == 0;
}

size_t queue_size(const queue* q) {
    if (q == NULL) return 0;
    return q->len;
}

void queue_clear(queue* q) {
    if (q == NULL) return;

    node* current = q->first;

    while (current != NULL) {
        node* next = current->next;
        if (q->destroy != NULL && current->value != NULL) {
            q->destroy(current->value);
        }
        free(current);
        current = next;
    }

    q->first = NULL;
    q->last = NULL;
    q->len = 0;
}

void queue_print(const queue* q) {
    if (q == NULL || q->len == 0 || q->print == NULL) {
        return;
    }

    node* current = q->first;

    while (current != NULL) {
        q->print(current->value);
        if (current->next != NULL) printf(" -> ");
        current = current->next;
    }
}

void queue_free(queue* q) {
    if (q == NULL) return;
    queue_clear(q);
    free(q);
}