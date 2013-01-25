#ifndef LIST_H
#define LIST_H
#include <stdbool.h>

//our list type that we'll use throughout 
//along with the helper functions: newitem and append
//this is boring, go down to main
typedef struct list list;

struct list {
  void *val;
  list *next;
};

list *newitem(void *v); 

list *copyitem(list *i); 

list *append(list *l, void *v); 

list *concat(list *h, list *t); 

list *copy(list *h); 

void freelist(list *l, bool objs); 

#endif

