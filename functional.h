#ifndef FUNCTIONAL_H
#define FUNCTIONAL_H
#include "list.h"

void iter(list *l, void (*fn)(void *, void *), void *args); 

list *map(list *l, void *(*fn)(void *, void *), void *args); 

list *filter(list *l, bool (*fn)(void *, void *), void *args); 

list *range(int start, int end); 


#endif

