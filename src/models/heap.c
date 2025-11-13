#include "heap.h"
#include <stdlib.h>

struct Heap {
    void** data;
    size_t capacity;
    size_t size;
    HeapCompare cmp;
};

static bool heap_grow(Heap* h) {
    size_t new_cap = h->capacity ? h->capacity * 2 : 8;
    void** new_data = (void**)realloc(h->data, new_cap * sizeof(void*));
    if (!new_data) return false;
    h->data = new_data;
    h->capacity = new_cap;
    return true;
}

static void heap_swap(void** a, void** b) {
    void* t = *a; *a = *b; *b = t;
}

static void sift_up(Heap* h, size_t idx) {
    while (idx > 0) {
        size_t parent = (idx - 1) / 2;
        if (h->cmp(h->data[idx], h->data[parent]) >= 0) break;
        heap_swap(&h->data[idx], &h->data[parent]);
        idx = parent;
    }
}

static void sift_down(Heap* h, size_t idx) {
    while (1) {
        size_t left = idx * 2 + 1;
        size_t right = idx * 2 + 2;
        size_t smallest = idx;
        if (left < h->size && h->cmp(h->data[left], h->data[smallest]) < 0) smallest = left;
        if (right < h->size && h->cmp(h->data[right], h->data[smallest]) < 0) smallest = right;
        if (smallest == idx) break;
        heap_swap(&h->data[idx], &h->data[smallest]);
        idx = smallest;
    }
}

Heap* heap_create(size_t initial_capacity, HeapCompare compare) {
    if (!compare) return NULL;
    Heap* h = (Heap*)malloc(sizeof(Heap));
    if (!h) return NULL;
    if (initial_capacity == 0) initial_capacity = 8;
    h->data = (void**)malloc(initial_capacity * sizeof(void*));
    if (!h->data) {
        free(h);
        return NULL;
    }
    h->capacity = initial_capacity;
    h->size = 0;
    h->cmp = compare;
    return h;
}

void heap_destroy(Heap* h) {
    if (!h) return;
    free(h->data);
    free(h);
}

bool heap_push(Heap* h, void* item) {
    if (!h) return false;
    if (h->size == h->capacity) {
        if (!heap_grow(h)) return false;
    }
    h->data[h->size] = item;
    sift_up(h, h->size);
    h->size++;
    return true;
}

void* heap_pop(Heap* h) {
    if (!h || h->size == 0) return NULL;
    void* top = h->data[0];
    h->size--;
    if (h->size > 0) {
        h->data[0] = h->data[h->size];
        sift_down(h, 0);
    }
    return top;
}

void* heap_peek(Heap* h) {
    if (!h || h->size == 0) return NULL;
    return h->data[0];
}

size_t heap_size(Heap* h) {
    return h ? h->size : 0;
}

bool heap_is_empty(Heap* h) {
    return !h || h->size == 0;
}
