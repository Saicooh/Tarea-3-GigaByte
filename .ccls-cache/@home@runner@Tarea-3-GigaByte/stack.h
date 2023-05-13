#ifndef Stack_h
#define Stack_h

#include "list.h"

typedef List Stack;

#define createStack() createList()
#define stack_pop(stack) popFront(stack) 
#define stack_push(stack,data) pushFront(stack,data) 
#define stack_top(stack) firstList(stack) 
#define stack_clean(stack) cleanList(stack)
#define stack_clone(stack) List_clone(stack)

#endif /* Stack.h */
