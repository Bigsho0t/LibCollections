#pragma once

typedef struct node {
    void* value;
    struct node* next;
} node;

node* new_node(void* value);