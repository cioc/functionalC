#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

//our list type that we'll use throughout 
//along with the helper functions: newitem and append
//this is boring, go down to main
typedef struct list list;

struct list {
  void *val;
  list *next;
};

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

// a function to play with iter
void
printint(void *v, void *args) {
  printf("%d\n", (int)(*(int *)v));
}

//returns true if int v is odd, false otherwise
bool
odd(void *v, void *args) {
  return (bool)(*((int *)v) % 2);
}

//returns twice int v
//boy is this terrible
void *
dbl(void *v, void *args) {
  int *o = (int *)malloc(sizeof(int)); 
  *o = *((int *)v) * 2;
  return o;
}

int
main(int argc, char **argv) {
  //LET THE MEMORY LEAKING BEGIN!!!
  iter(map(range(0, 10), dbl,NULL), printint, NULL);
  iter(filter(range(0, 10), odd, NULL), printint, NULL); 
  exit(0);
}
