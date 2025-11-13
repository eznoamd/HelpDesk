#ifndef HEAP_H
#define HEAP_H

#include <stddef.h>
#include <stdbool.h>

typedef int (*HeapCompare)(const void*, const void*);

typedef struct Heap Heap;

Heap* heap_create(size_t initial_capacity, HeapCompare compare);
void heap_destroy(Heap* h);
bool heap_push(Heap* h, void* item);
void* heap_pop(Heap* h);
void* heap_peek(Heap* h);
size_t heap_size(Heap* h);
bool heap_is_empty(Heap* h);

#endif