#ifndef CLOSURE_H
#define CLOSURE_H
#include <unistd.h>

typedef struct closure closure;

struct closure {
  void *(*fn)(list *);
  list *env;
};

typedef struct envobj envobj;

struct envobj {
  void *val;
  ssize_t size;
};

envobj *envitem(void *var, ssize_t size);

void *unbox(list *l); 

closure *bind(closure *c, void *(*fn)(list *), envobj *env);

void *call(closure *c, envobj *env);

envobj *liftint(int a); 


#endif 
