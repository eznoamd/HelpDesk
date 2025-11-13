#include "fifo.h"
#include <stdlib.h>

struct FIFO {
    void** data;
    size_t capacity;
    size_t head;
    size_t tail;
    size_t size;
};

static bool fifo_grow(FIFO* q) {
    size_t new_cap = q->capacity ? q->capacity * 2 : 8;
    void** new_data = (void**)malloc(new_cap * sizeof(void*));
    if (!new_data) return false;
    for (size_t i = 0; i < q->size; ++i) {
        new_data[i] = q->data[(q->head + i) % q->capacity];
    }
    free(q->data);
    q->data = new_data;
    q->capacity = new_cap;
    q->head = 0;
    q->tail = q->size;
    return true;
}

FIFO* fifo_create(size_t initial_capacity) {
    FIFO* q = (FIFO*)malloc(sizeof(FIFO));
    if (!q) return NULL;
    if (initial_capacity == 0) initial_capacity = 8;
    q->data = (void**)malloc(initial_capacity * sizeof(void*));
    if (!q->data) {
        free(q);
        return NULL;
    }
    q->capacity = initial_capacity;
    q->head = 0;
    q->tail = 0;
    q->size = 0;
    return q;
}

void fifo_destroy(FIFO* q) {
    if (!q) return;
    free(q->data);
    free(q);
}

bool fifo_enqueue(FIFO* q, void* item) {
    if (!q) return false;
    if (q->size == q->capacity) {
        if (!fifo_grow(q)) return false;
    }
    q->data[q->tail] = item;
    q->tail = (q->tail + 1) % q->capacity;
    q->size++;
    return true;
}

void* fifo_dequeue(FIFO* q) {
    if (!q || q->size == 0) return NULL;
    void* item = q->data[q->head];
    q->head = (q->head + 1) % q->capacity;
    q->size--;
    return item;
}

void* fifo_peek(FIFO* q) {
    if (!q || q->size == 0) return NULL;
    return q->data[q->head];
}

size_t fifo_size(FIFO* q) {
    return q ? q->size : 0;
}

bool fifo_is_empty(FIFO* q) {
    return !q || q->size == 0;
}
