#pragma once

#include <stddef.h>
#include <stdbool.h>

typedef enum {
    SUCCESS,
    ALLOC_FAIL,
    NULL_POINTER,
    DUPLICATE,
    NOT_FOUND,
    OUT_OF_BOUNDS,
    UNDERFLOW,
    OUT_OF_RANGE,
    KEY_ALREADY_EXISTS
} status;

typedef void (*free_func)(void*);
typedef void (*print_func)(const void*);
typedef int (*cmp_func)(const void*, const void*);
typedef size_t (*hash_func)(const void*);