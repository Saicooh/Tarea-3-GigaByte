#ifndef Heap_h
#define Heap_h
#include "stack.h"

#define MAX 31

typedef struct Heap Heap;

Heap* createHeap();
void* heap_top(Heap* pq);
void* heap_top_priority(Heap* pq);
void heap_push(Heap* pq, void* data, int priority);
void heap_pop(Heap* pq);
void heap_pop_especifico(Heap* pq, int i);
void heap_remove(Heap *pq, void *tareaBuscar);
int get_size(Heap *pq);
Heap* heap_clone(Heap* pq);
void* get_data(Heap* pq, int pos);

#endif /* HashMap_h */

