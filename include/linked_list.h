#pragma once

#include "types.h"

typedef struct list list;

list* new_list(free_func destroy, cmp_func cmp, print_func print);
status list_add_first(list* l, void* value);
status list_add_last(list* l, void* value);
status list_add(list* l, void* value, size_t index);
status list_remove_value(list* l, const void* value);
status list_remove_index(list* l, size_t index);
void* list_get(const list* l, size_t index);
//talvez um set
//talvez indexof
bool list_contains(const list* l, const void* value);
bool list_is_empty(const list* l);
size_t list_size(const list* l);
void list_clear(list* l);
void list_print(const list* l);
void list_free(list* l);