#include "node.h"
#include <stdlib.h>

node* new_node(void* value) {
    node* n = malloc(sizeof(*n));

    if (n == NULL) {
        return NULL;
    }

    n->value = value;
    n->next = NULL;
    
    return n;
}