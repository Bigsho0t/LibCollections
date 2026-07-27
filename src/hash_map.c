#include "hash_map.h"
#include "hash_config.h"
#include "hash_utils.h"
#include <stdlib.h>

typedef struct hash_map_node {
    void* key;
    size_t hash;
    void* value;
    struct hash_map_node* next;
} hash_map_node;

typedef struct hash_map {
    hash_map_node** buckets;
    size_t cap;
    size_t len;
    hash_func hash;
    cmp_func cmp_key;
    free_func destroy_key;
    free_func destroy_value;
} hash_map;

static hash_map_node* new_hash_map_node(void* key, size_t hash, void* value) {
    hash_map_node* n = malloc(sizeof(*n));

    if (n == NULL) {
        return NULL;
    }

    n->key = key;
    n->hash = hash;
    n->value = value;
    n->next = NULL;

    return n;
}

static status hash_map_resize(hash_map* hm) {
    size_t new_cap = next_prime(hm->cap * 2);
    hash_map_node** new_buckets = calloc(new_cap, sizeof(*new_buckets));

    if (new_buckets == NULL) {
        return ALLOC_FAIL;
    }

    for (size_t i = 0; i < hm->cap; i++) {
        hash_map_node* current = hm->buckets[i];

        while (current != NULL) {
            hash_map_node* next = current->next;
            size_t new_index = current->hash % new_cap;
            current->next = new_buckets[new_index];
            new_buckets[new_index] = current;
            current = next;
        }
    }

    free(hm->buckets);
    hm->buckets = new_buckets;
    hm->cap = new_cap;

    return SUCCESS;
}

hash_map* new_hash_map(hash_func hash, cmp_func cmp_key, free_func destroy_key, free_func destroy_value) {
    if (hash == NULL || cmp_key == NULL) {
        return NULL;
    }

    hash_map* hm = malloc(sizeof(*hm));

    if (hm == NULL) {
        return NULL;
    }

    hm->buckets = calloc(HASH_INITIAL_CAPACITY, sizeof(hash_map_node*));

    if (hm->buckets == NULL) {
        free(hm);
        return NULL;
    }

    hm->cap = HASH_INITIAL_CAPACITY;
    hm->len = 0;
    hm->hash = hash;
    hm->cmp_key = cmp_key;
    hm->destroy_key = destroy_key;
    hm->destroy_value = destroy_value;

    return hm;
}

status hash_map_put(hash_map* hm, void* key, void* value) {
    if (hm == NULL || key == NULL) {
        return NULL_POINTER;
    }

    size_t hash = hm->hash(key);
    size_t index = hash % hm->cap;

    for (hash_map_node* current = hm->buckets[index]; current != NULL; current = current->next) {
        if (current->hash == hash && hm->cmp_key(current->key, key) == 0) {
             
            if (hm->destroy_value != NULL && current->value != value) {
                hm->destroy_value(current->value);
            }

            current->value = value;

            return SUCCESS;
        }
    }

    if ((double)(hm->len + 1) / hm->cap > HASH_LOAD_FACTOR) {
        status status = hash_map_resize(hm);

        if (status != SUCCESS) {
            return status;
        }

        index = hash % hm->cap;
    }

    hash_map_node* node = new_hash_map_node(key, hash, value);

    if (node == NULL) {
        return ALLOC_FAIL;
    }

    node->next = hm->buckets[index];
    hm->buckets[index] = node;

    hm->len++;

    return SUCCESS;
}

status hash_map_remove(hash_map* hm, const void* key) {
    if (hm == NULL || key == NULL) {
        return NULL_POINTER;
    }

    if (hm->len == 0) {
        return UNDERFLOW;
    }

    size_t hash = hm->hash(key);
    size_t index = hash % hm->cap;

    hash_map_node* current = hm->buckets[index];
    hash_map_node* previous = NULL;

    while (current != NULL) {
        if (current->hash == hash && hm->cmp_key(current->key, key) == 0) {

            if (current == hm->buckets[index]) {
                hm->buckets[index] = current->next;
            } else {
                previous->next = current->next;
            }

            if (hm->destroy_key != NULL) {
                hm->destroy_key(current->key);
            }

            if (hm->destroy_value != NULL && current->value != NULL) {
                hm->destroy_value(current->value);
            }

            free(current);
            hm->len--;

            return SUCCESS;
        }

        previous = current;
        current = current->next;
    }

    return NOT_FOUND;
}

void* hash_map_get(const hash_map* hm, const void* key) {
    if (hm == NULL || hm->cap == 0 || key == NULL) {
        return NULL;
    }

    size_t hash = hm->hash(key);
    size_t index = hash % hm->cap;

    for (hash_map_node* current = hm->buckets[index]; current != NULL; current = current->next) {
        if (current->hash == hash && hm->cmp_key(current->key, key) == 0) {
            return current->value;
        }
    }

    return NULL;
}

bool hash_map_contains(const hash_map* hm, const void* key) {
    if (hm == NULL || key == NULL) {
        return false;
    }

    size_t hash = hm->hash(key);
    size_t index = hash % hm->cap;

    for (hash_map_node* current = hm->buckets[index]; current != NULL; current = current->next) {
        if (current->hash == hash && hm->cmp_key(current->key, key) == 0) {
            return true;
        }
    }

    return false;
}

bool hash_map_is_empty(const hash_map* hm) {
    if (hm == NULL) return true;
    return hm->len == 0;
}

size_t hash_map_size(const hash_map* hm) {
    if (hm == NULL) return 0;
    return hm->len;
}

const void** hash_map_values(const hash_map* hm) {
    if (hm == NULL || hm->len == 0) {
        return NULL;
    }

    const void** values = malloc(hm->len * sizeof(const void*));
    
    if (values == NULL) {
        return NULL;
    }

    size_t idx = 0;
    for (size_t i = 0; i < hm->cap; i++) {
        hash_map_node* current = hm->buckets[i];

        while (current != NULL) {
            values[idx] = current->value;
            idx++;
            current = current->next;
        }
    }

    return values;
}

const void** hash_map_keys(const hash_map* hm) {
    if (hm == NULL || hm->len == 0) {
        return NULL;
    }

    const void** keys = malloc(hm->len * sizeof(const void*));
    
    if (keys == NULL) {
        return NULL;
    }

    size_t idx = 0;
    for (size_t i = 0; i < hm->cap; i++) {
        hash_map_node* current = hm->buckets[i];

        while (current != NULL) {
            keys[idx] = current->key;
            idx++;
            current = current->next;
        }
    }

    return keys;
}

void hash_map_clear(hash_map* hm) {
    if (hm == NULL) return;
    
    for (size_t i = 0; i < hm->cap; i++) {
        hash_map_node* current = hm->buckets[i];

        while (current != NULL) {
            hash_map_node* next = current->next;
            if (hm->destroy_key != NULL) {
                hm->destroy_key(current->key);
            }
            if (hm->destroy_value != NULL && current->value != NULL) {
                hm->destroy_value(current->value);
            }

            free(current);
            current = next;
        }

        hm->buckets[i] = NULL;
    }

    hm->len = 0;
}

void hash_map_free(hash_map* hm) {
    if (hm == NULL) return;
    hash_map_clear(hm);
    free(hm->buckets);
    free(hm);
}