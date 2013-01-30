#include <stdlib.h>
#include <stdio.h>
#include "gc.h"
#include "closure.h"
#include "list.h"

typedef struct ref ref;
struct ref {
  void *ptr; //pointer to the obj 
  TYPE type; //obj type
  ref *next; //refs are stored as a list;
};

typedef struct gc gc;
struct gc {
  ref *marked;   //these will not be freed unless unmarked
  ref *last_marked;
  ref *unmarked; //these will be freed when gc_collect is called 
  ref *last_unmarked; //we want append to be O(1)
  void (*destructor_table[TYPE_COUNT])(void *);
};

//this IS the garbage collector
static gc _gc;

//private functions
void gc_register_destructor(TYPE, void (*)(void *));
ref *refitem(void *, TYPE);
void append_unmarked(ref *unmarked);
void append_marked(ref *marked);
ref *remove_unmarked(void *obj);
ref *remove_marked(void *obj);
void standard_free(void *ptr);

void
gc_register_destructor(TYPE type, void (*destructor)(void *)) {
  _gc.destructor_table[type] = destructor;
}

ref *
refitem(void *ptr, TYPE type) {
  ref *o = malloc(sizeof(ref));
  o->ptr = ptr;
  o->type = type;
  o->next = NULL;
  return o;
}

void 
append_unmarked(ref *unmarked) {
  if (_gc.unmarked == NULL) {
    _gc.unmarked = unmarked; 
    _gc.last_unmarked = _gc.unmarked;
  }
  else {
    _gc.last_unmarked->next = unmarked;
    _gc.last_unmarked = unmarked; 
  }
}

ref *
remove_unmarked(void *obj) {
  ref *curr = _gc.unmarked;
  if (curr == NULL) {
    return NULL;
  }
  if (curr->ptr == obj) {
    _gc.unmarked = curr->next;
    curr->next = NULL;
    return curr;  
  }
  if (_gc.last_unmarked->ptr == obj) {
    for (; curr->next->ptr != obj; curr = curr->next)
      ;
    ref *o = _gc.last_unmarked;
    o->next = NULL;
    _gc.last_unmarked = curr;
    curr->next = NULL; 
    return o;     
  }
  for (; curr->next != NULL; curr = curr->next) {
    if (curr->next->ptr == obj) {
      ref *o = curr->next;
      curr->next = o->next;
      o->next = NULL;
      return o;
    }
  }
  return NULL;
}

ref *
remove_marked(void *obj) {
  ref *curr = _gc.marked;
  if (curr == NULL) {
    return NULL;
  }
  if (curr->ptr == obj) {
    _gc.marked = curr->next;
    curr->next = NULL;
    return curr;  
  }
  if (_gc.last_marked->ptr == obj) {
    for (; curr->next->ptr != obj; curr = curr->next)
      ;
    ref *o = _gc.last_marked;
    o->next = NULL;
    _gc.last_marked = curr;
    curr->next = NULL; 
    return o;     
  }
  for (; curr->next != NULL; curr = curr->next) {
    if (curr->next->ptr == obj) {
      ref *o = curr->next;
      curr->next = o->next;
      o->next = NULL;
      return o;
    }
  }
  return NULL;
}

void
append_marked(ref *marked) {
  if (_gc.marked == NULL) {
    _gc.marked = marked; 
    _gc.last_marked = _gc.marked;
  }
  else {
    _gc.last_marked->next = marked;
    _gc.last_marked = marked; 
  }
}

void
standard_free(void *ptr) {
  free(ptr);
}

//public functions
void
gc_init(void) {
  //register destructors here
  //all of these are basically just standard free
  //just showing that one could register other destructors
  gc_register_destructor(ENVOBJ, envobj_free);
  gc_register_destructor(CLOSURE, closure_free);
  gc_register_destructor(LIST, list_free);
  gc_register_destructor(STANDARD, standard_free); 
  //don't change these
  _gc.marked = NULL;
  _gc.unmarked = NULL;
  _gc.last_unmarked = NULL;
  _gc.last_marked = NULL;
}

void
gc_remove(void *obj) {
  ref *r1 = remove_unmarked(obj);
  if (r1 != NULL) {
    free(r1);
  }
  ref *r2 = remove_marked(obj);
  if (r2 != NULL) {
    free(r2);
  }
}

void
gc_mark(void *obj) {
  ref *r = remove_unmarked(obj); 
  append_marked(r);
}

void
gc_unmark(void *obj) {
  ref *r = remove_marked(obj);
  append_unmarked(r);
}

//don't register objs twice; boy that could go poorly
void
gc_register(void *obj, TYPE type) {
  ref *n = refitem(obj, type);
  append_unmarked(n); 
}

void
gc_collect(void) {
  ref *curr = _gc.unmarked;
  while (curr != NULL) {
    ref *next = curr->next;
    (*(_gc.destructor_table[curr->type]))(curr->ptr);
    free(curr);
    curr = next;   
  }
  _gc.unmarked = NULL;
}

//displays everything inside the garbage collector
//for debugging purposes
void
gc_print(void) {
  ref *curr;
  printf("TO BE COLLECTED (UNMARKED):\n");
  for (curr = _gc.unmarked; curr != NULL; curr = curr->next) {
    switch(curr->type) {
      case LIST:
        printf("LIST at %p\n", curr->ptr);
      break;
      case STANDARD:
        printf("STANDARD at %p\n", curr->ptr);
      break;
      case ENVOBJ:
        printf("ENVOBJ at %p\n", curr->ptr);
      break;
      case CLOSURE:
        printf("CLOSURE at %p\n", curr->ptr);
      break; 
    }
  }
  printf("MARKED FOR SAFE KEEPING:\n");
  for (curr = _gc.marked; curr != NULL; curr = curr->next) {
    switch(curr->type) {
      case LIST:
        printf("LIST at %p\n", curr->ptr);
      break;
      case STANDARD:
        printf("STANDARD at %p\n", curr->ptr);
      break;
      case ENVOBJ:
        printf("ENVOBJ at %p\n", curr->ptr);
      break;
      case CLOSURE:
        printf("CLOSURE at %p\n", curr->ptr);
      break; 
    }
  }
}
