#include <stdlib.h>
#include <stdio.h>
#include "list.h"
#include "functional.h"

//some standard functional programming functions
void
iter(list *l, void (*fn)(void *, void *), void *args) {
  list *curr;
  for (curr = l; curr != NULL; curr = curr->next) {
    (*fn)(curr->val, args); 
  }
}

list *
map(list *l, void *(*fn)(void *, void *), void *args) {
  list *o = NULL;
  list *curr;
  for (curr = l; curr != NULL; curr = curr->next) {
    o = append(o, (*fn)(curr->val, args));
  }
  return o;
}

list *
filter(list *l, bool (*fn)(void *, void *), void *args) {
  list *o = NULL;
  list *curr;
  for (curr = l; curr != NULL; curr = curr->next) {
    if ((*fn)(curr->val, args)) {
      o = append(o, copyitem(curr->val));
    }
  }
  return o;
}

//not lazy
list *
range(int start, int end) {
  list *o = NULL;
  for (int i = start; i <= end; ++i) {
    int *aloc = (int *)malloc(sizeof(int));
    *aloc = i;  
    o = append(o, (void *)aloc);
  }
  return o;
}


