#pragma once

#include "types.h"

typedef struct hash_set hash_set;

hash_set* new_hash_set(hash_func hash, cmp_func cmp, free_func destroy);
status hash_set_add(hash_set* hs, void* value);
status hash_set_remove(hash_set* hs, const void* value);
bool hash_set_contains(const hash_set* hs, const void* value);
bool hash_set_is_empty(const hash_set* hs);
size_t hash_set_size(const hash_set* hs);
const void** hash_set_values(const hash_set* hs);
void hash_set_clear(hash_set* hs);
void hash_set_free(hash_set* hs);