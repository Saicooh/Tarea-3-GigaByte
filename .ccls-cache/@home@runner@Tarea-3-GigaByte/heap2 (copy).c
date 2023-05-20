#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "heap.h"
#include "stack.h"

typedef struct nodoA
{
  void *data;
  int priority;
} heapElem;

typedef struct Heap
{
  heapElem *heapArray;
  int size;
  int capac;
} Heap;

typedef struct
{
  char nombre[MAX];
  bool completada;
  Stack *tareasPrecedentes; // Creamos una pila de tareas precedentes.
  Stack *registro;
} Tarea; // Definimos nuestra estructura a trabajar.

Heap *createHeap()
{
  Heap *h = malloc(sizeof(Heap));

  h -> heapArray = malloc(3 * sizeof(Heap));
  h -> size = 0;
  h -> capac = 3;

  return h;
}

void *heap_top(Heap* pq)
{
  if(!pq -> size) return NULL;

  return pq -> heapArray[0].data;
}

void *heap_top_priority(Heap* pq)
{
  if(!pq -> size) return NULL;

  return pq -> heapArray[0].priority;
}

void heap_push(Heap *pq, void *data, int priority)
{
  int i = pq -> size;
  
  if(pq -> size == pq -> capac)
  {
    pq -> capac = pq -> capac * 2 + 1;
    pq -> heapArray = realloc(pq -> heapArray, pq -> capac * sizeof(heapElem));
  }
  
  while (i > 0 && priority > pq -> heapArray[(i - 1) / 2].priority)
  {
    pq -> heapArray[i] = pq -> heapArray[(i - 1) / 2];
    i = (i - 1) / 2;
  }

  pq -> heapArray[i].data = data;
  pq -> heapArray[i].priority = priority;
  pq -> size++;
}


/*void heap_push(Heap* pq, void* data, int priority){
  if(pq->size == pq->capac)
  {
    pq->capac = pq->capac * 2 + 1;
    pq->heapArray = (heapElem *) realloc(pq->heapArray, pq->capac * sizeof(heapElem));
  }

  int pos = pq->size;

  while(pos > 0 && pq->heapArray[(pos-1)/2].priority < priority){
    pq->heapArray[pos] = pq->heapArray[(pos-1)/2];
    pos = (pos-1)/2;
  }

  pq->heapArray[pos].priority = priority;
  pq->heapArray[pos].data = data;
  pq->size++;
}*/



void heap_pop(Heap *pq)
{
  int hijoMenor = 1, hijoMayor = 2, pos = 0;
  heapElem auxiliar;
  
  pq -> size--;
  pq -> heapArray[0] = pq -> heapArray[pq -> size];
  
  while (hijoMenor < pq -> size)
  {
    int nodoHijo = hijoMenor;
    
    if (hijoMenor < pq -> size && pq -> heapArray[hijoMayor].priority > pq -> heapArray[hijoMenor].priority) nodoHijo = hijoMayor;
    //if (hijoMenor < pq -> size && pq -> heapArray[hijoMayor].priority < pq -> heapArray[hijoMenor].priority) nodoHijo = hijoMayor;
    
    if(pq -> heapArray[pos].priority >= pq -> heapArray[nodoHijo].priority) break;
    
    auxiliar = pq -> heapArray[pos];
    pq -> heapArray[pos] = pq -> heapArray[nodoHijo];
    pq -> heapArray[nodoHijo] = auxiliar;
    pos = nodoHijo;
    hijoMenor = hijoMenor * 2 + 1;
    hijoMayor = hijoMayor * 2 + 2;
  }
}

void heap_remove(Heap *pq, void *tareaBuscar)
{
  int i;
  int index = -1;
  
  for (i = 0; i < pq->size; i++)
  {
    Tarea *t = (Tarea *) pq->heapArray[i].data;
    
    if (strcmp(t->nombre, tareaBuscar) == 0) 
    {
      index = i;
      pq->size--;
      break;
    }
  }

  if (index == -1) {
    // La tarea especificada no se encontró en el heap
    return;
  }

  // Reemplazar la tarea a eliminar con la última tarea en el heap
  pq->heapArray[index] = pq->heapArray[pq->size];
  
  // Restaurar las propiedades del heap
  while (index > 0 && pq->heapArray[(index - 1) / 2].priority > pq->heapArray[index].priority) {
    heapElem aux = pq->heapArray[index];
    pq->heapArray[index] = pq->heapArray[(index - 1) / 2];
    pq->heapArray[(index - 1) / 2] = aux;
    index = (index - 1) / 2;
  }
  heap_pop(pq);
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
