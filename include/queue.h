#pragma once

#include "types.h"

typedef struct queue queue;

queue* new_queue(free_func destroy, print_func print);
status queue_enqueue(queue* q, void* value);
void* queue_dequeue(queue* q);
const void* queue_front(const queue* q);
bool queue_is_empty(const queue* q);
size_t queue_size(const queue* q);
void queue_clear(queue* q);
void queue_print(const queue* q);
void queue_free(queue* q);