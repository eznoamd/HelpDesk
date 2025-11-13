#ifndef FIFO_H
#define FIFO_H

#include <stddef.h>
#include <stdbool.h>

typedef struct FIFO FIFO;

FIFO* fifo_create(size_t initial_capacity);
void fifo_destroy(FIFO* q);
bool fifo_enqueue(FIFO* q, void* item);
void* fifo_dequeue(FIFO* q);
void* fifo_peek(FIFO* q);
size_t fifo_size(FIFO* q);
bool fifo_is_empty(FIFO* q);

#endif
