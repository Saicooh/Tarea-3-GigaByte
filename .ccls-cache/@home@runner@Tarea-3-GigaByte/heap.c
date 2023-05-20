#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "heap.h"

typedef struct nodo{
   void* data;
   int priority;
}heapElem;

typedef struct Heap{
  heapElem* heapArray;
  int size;
  int capac;
} Heap;

typedef struct
{
  char nombre[MAX];
  bool completada;
  Stack *tareasPrecedentes;
  Stack *registro;
} Tarea; 

void *heap_top_priority(Heap* pq)
{
  if(!pq -> size) return NULL;
  return pq -> heapArray[0].priority;
}

Heap *heap_clone(Heap *pq)
{
  if (pq == NULL) return NULL;

  Heap *clon = createHeap();
  clon->size = pq->size;
  clon->capac = pq->capac;

  clon->heapArray = (heapElem*) malloc(sizeof(heapElem) * pq->capac);

  int i;
  for (i = 0; i < pq->size; i++) 
  {
    clon->heapArray[i] = pq->heapArray[i];
  }

  return clon;
}

void heap_remove(Heap *pq, void *tareaBuscar)
{
  int i;
  int index = -1;
  
  for (i = 0; i < pq->size; i++)
  {
    if (pq -> heapArray[i].data == tareaBuscar) 
    {
      index = i;
      pq -> size--;
      break;
    }
  }

  if (index == -1) return;

  pq -> heapArray[index] = pq -> heapArray[pq -> size];
  
  while (index > 0 && pq -> heapArray[(index - 1) / 2].priority > pq -> heapArray[index].priority) 
  {
    heapElem aux = pq->heapArray[index];
    pq->heapArray[index] = pq->heapArray[(index - 1) / 2];
    pq->heapArray[(index - 1) / 2] = aux;
    index = (index - 1) / 2;
  }
}

Heap *createHeap()
{
   Heap *pq = (Heap*) malloc(sizeof(Heap));
   pq->heapArray = (heapElem*) malloc(3*sizeof(heapElem));
   pq->size = 0;
   pq->capac = 3; 
   return pq;
}

void *heap_top(Heap* pq)
{
  if(pq->size == 0) return NULL;
  return pq->heapArray[0].data;
}

void heap_push(Heap* pq, void* data, int priority)
{
  if(pq->size+1>pq->capac)
  {
    pq->capac=(pq->capac)*2+1;
    pq->heapArray=realloc(pq->heapArray, (pq->capac)*sizeof(heapElem));
  }

  int now = pq->size;
  
  while(now > 0 && pq->heapArray[(now-1)/2].priority < priority)
  {
    pq->heapArray[now] = pq->heapArray[(now-1)/2];
    now = (now - 1) / 2;
  }
  
  pq->heapArray[now].priority = priority;
  pq->heapArray[now].data = data;
  pq->size++;
  
}

int get_size(Heap* pq)
{
  return pq->size;
}

void heap_pop(Heap* pq)
{
  pq->size--;
  pq->heapArray[0] = pq->heapArray[pq->size];
  int priority=pq->heapArray[0].priority;

  int now = 1;
  
  while((now<=pq->size && pq->heapArray[now].priority > priority) || (now+1<=pq->size && pq->heapArray[now+1].priority > priority))
  {
    heapElem tmp=pq->heapArray[(now-1)/2];
    if(now+1<=pq->size && pq->heapArray[now].priority < pq->heapArray[now+1].priority) now++;

    pq->heapArray[(now-1)/2]=pq->heapArray[now];
    pq->heapArray[now]=tmp;

    now = now * 2 + 1;
  }
}
