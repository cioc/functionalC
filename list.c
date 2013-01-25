#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "list.h"

list *
newitem(void *v) {
  list *o = (list *)malloc(sizeof(list));
  if (o == NULL) {
    exit(1);
  }
  o->val = v;
  o->next = NULL;
  return o;
}

list *
copyitem(list *i) {
  list *o = (list *)malloc(sizeof(list));
  if (o == NULL) {
    exit(1);
  }
  o->val = i->val;
  o->next = NULL;
  return o;
}

list *
append(list *l, void *v) {
  list *ni = newitem(v);
  if (l == NULL) {
    return ni;
  }
  list *curr;
  for (curr = l; curr->next != NULL; curr = curr->next)
    ;
  curr->next = ni;
  return l;
}

list *
concat(list *h, list *t) {
  list *curr;
  for (curr = h; curr->next != NULL; curr = curr->next)
	  ;
  curr->next = t;
	return h;	
}

//objs flag set to true will also free the objects in the list
void
freelist(list *l, bool objs) {
  list *curr = l;
  while (curr != NULL) {
    list *n = curr->next;
    if (objs) {
      free(curr->val);
    }
    free(curr);
    curr = n;
  }
}


