#pragma once

#include "types.h"

typedef struct hash_map hash_map;

hash_map* new_hash_map(hash_func hash, cmp_func cmp_key, free_func destroy_key, free_func destroy_value);
status hash_map_put(hash_map* hm, void* key, void* value);
status hash_map_remove(hash_map* hm, const void* key);
void* hash_map_get(const hash_map* hm, const void* key);
bool hash_map_contains(const hash_map* hm, const void* key);
bool hash_map_is_empty(const hash_map* hm);
size_t hash_map_size(const hash_map* hm);
const void** hash_map_values(const hash_map* hm);
const void** hash_map_keys(const hash_map* hm);
void hash_map_clear(hash_map* hm);
void hash_map_free(hash_map* hm);