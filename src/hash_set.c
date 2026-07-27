#include "hash_set.h"
#include "hash_config.h"
#include "hash_utils.h"
#include <stdlib.h>

typedef struct hash_set_node {
    void* value;
    size_t hash;
    struct hash_set_node* next;
} hash_set_node;

typedef struct hash_set {
    hash_set_node** buckets;
    size_t cap;
    size_t len;
    hash_func hash;
    cmp_func cmp;
    free_func destroy;
} hash_set;

static hash_set_node* new_hash_set_node(void* value, size_t hash) {
    hash_set_node* n = malloc(sizeof(*n));

    if (n == NULL) {
        return NULL;
    }

    n->value = value;
    n->hash = hash;
    n->next = NULL;

    return n;
}

static status hash_set_resize(hash_set* hs) {
    size_t new_cap = next_prime(hs->cap * 2 + 1);
    hash_set_node** new_buckets = calloc(new_cap, sizeof(hash_set_node*));

    if (new_buckets == NULL) {
        return ALLOC_FAIL;
    }

    for (size_t i = 0; i < hs->cap; i++) {
        hash_set_node* current = hs->buckets[i];

        while (current != NULL) {
            hash_set_node* next = current->next;
            size_t new_index = current->hash % new_cap;

            current->next = new_buckets[new_index];
            new_buckets[new_index] = current;

            current = next;
        }
    }

    free(hs->buckets);
    hs->buckets = new_buckets;
    hs->cap = new_cap;

    return SUCCESS;
}

hash_set* new_hash_set(hash_func hash, cmp_func cmp, free_func destroy) {
    if (hash == NULL || cmp == NULL) {
        return NULL;
    }

    hash_set* hs = malloc(sizeof(*hs));

    if (hs == NULL) {
        return NULL;
    }

    hs->buckets = calloc(HASH_INITIAL_CAPACITY, sizeof(hash_set_node*));

    if (hs->buckets == NULL) {
        free(hs);
        return NULL;
    }

    hs->cap = HASH_INITIAL_CAPACITY;
    hs->len = 0;
    hs->hash = hash;
    hs->cmp = cmp;
    hs->destroy = destroy;
    return hs;
}

status hash_set_add(hash_set* hs, void* value) {
    if (hs == NULL || value == NULL) {
        return NULL_POINTER;
    }

    size_t hash = hs->hash(value);
    size_t index = hash % hs->cap;

    for (hash_set_node* current = hs->buckets[index]; current != NULL; current = current->next) {
        if (current->hash == hash && hs->cmp(current->value, value) == 0) {
            return DUPLICATE;
        }
    }

    if ((double)(hs->len + 1) / hs->cap > HASH_LOAD_FACTOR) {
        status resize_status = hash_set_resize(hs);
        if (resize_status != SUCCESS) {
            return resize_status;
        }
        
        index = hash % hs->cap;
    }

    hash_set_node* n = new_hash_set_node(value, hash);

    if (n == NULL) {
        return ALLOC_FAIL;
    }

    n->next = hs->buckets[index];
    hs->buckets[index] = n;
    hs->len++;

    return SUCCESS;
}

status hash_set_remove(hash_set* hs, const void* value) {
    if (hs == NULL || value == NULL) {
        return NULL_POINTER;
    }

    size_t hash = hs->hash(value);
    size_t index = hash % hs->cap;

    hash_set_node* current = hs->buckets[index];
    hash_set_node* prev = NULL;

    while (current != NULL) {
        if (current->hash == hash && hs->cmp(current->value, value) == 0) {
            if (current == hs->buckets[index]) {
                hs->buckets[index] = current->next;
            } else {
                prev->next = current->next;
            }

            if (hs->destroy != NULL) {
                hs->destroy(current->value);
            }

            free(current);
            hs->len--;

            return SUCCESS;
        }

        prev = current;
        current = current->next;
    }

    return NOT_FOUND;
}

bool hash_set_contains(const hash_set* hs, const void* value) {
    if (hs == NULL || value == NULL) {
        return false;
    }

    size_t hash = hs->hash(value);
    size_t index = hash % hs->cap;

    for (hash_set_node* current = hs->buckets[index]; current != NULL; current = current->next) {
        if (current->hash == hash && hs->cmp(current->value, value) == 0) {
            return true;
        }
    }

    return false;
}

bool hash_set_is_empty(const hash_set* hs) {
    if (hs == NULL) {
        return true;
    }

    return hs->len == 0;
}

size_t hash_set_size(const hash_set* hs) {
    if (hs == NULL) {
        return 0;
    }

    return hs->len;
}

const void** hash_set_values(const hash_set* hs) {
    if (hs == NULL || hs->len == 0) {
        return NULL;
    }

    const void** values = malloc(hs->len * sizeof(const void*));
    
    if (values == NULL) {
        return NULL;
    }

    size_t idx = 0;
    for (size_t i = 0; i < hs->cap; i++) {
        hash_set_node* current = hs->buckets[i];

        while (current != NULL) {
            values[idx] = current->value;
            idx++;
            current = current->next;
        }
    }

    return values;
}

void hash_set_clear(hash_set* hs) {
    if (hs == NULL) {
        return;
    }

    for (size_t i = 0; i < hs->cap; i++) {
        hash_set_node* current = hs->buckets[i];

        while (current != NULL) {
            hash_set_node* next = current->next;

            if (hs->destroy != NULL) {
                hs->destroy(current->value);
            }

            free(current);
            current = next;
        }

        hs->buckets[i] = NULL;
    }

    hs->len = 0;
}

void hash_set_free(hash_set* hs) {
    if (hs == NULL) {
        return;
    }

    hash_set_clear(hs);
    free(hs->buckets);
    free(hs);
}