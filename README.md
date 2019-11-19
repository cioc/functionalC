functionalC
===========

Not because it is good, but because we can... 

(Current blog: https://charlescary.com/)

e.g. Don't you always wish that you could write like this when you are programming in C?

```c
int
main(int argc, char **argv) {
  //yep, added first version of garbage collector
  gc_init(); //initialize the garbage collector

  iter(map(range(0, 10), dbl,NULL), printint, NULL);
  iter(filter(range(0, 10), odd, NULL), printint, NULL); 
  
  //Darker magic?  Not really...
  closure *addtwo = bind(NULL, add, liftint(2));
  closure *addten = bind(NULL, add, liftint(10));

  printf("%d\n", *(int *)call(addtwo, liftint(3)));
  printf("%d\n", *(int *)call(addten, liftint(3)));

  //all together now, with pseudo types everywhere woopie!!!
  list *vars = liftlist(range(0, 10), sizeof(int));
  list *res = lmap(vars, addtwo);
  iter(res, printint, NULL);

  gc_print(); //show eveything currently in the garbage collector

  gc_collect(); //you can guess what this does

  gc_print(); //anything left?
  
  //NOTE: The garbage collector is a work in progress.  
  //we're not tracking everything yet and 
  //the collector doesn't necessarily avoid double frees!
  //BEWARE!!!

  exit(0);
}

```

No?  Neither did I.  This is horrible.  

Functional C Programming Doc
============================

# The Basics

Our basic type is the list.

```c
struct list {
  void *val;
  list *next;
};
```

All of our functions use this type.  There are helper functions to work with lists like newitem, copyitem, and append.  Look at the code.

Some standard functional programming functions:

```c
//iterates through a list calling fn
void
iter(list *l, void (*fn)(void *, void *), void *args)
```
```c
//yeah, it's map
list *
map(list *l, void *(*fn)(void *, void *), void *args)
```

```c
//lifted map is so that you can map closures; maybe we'll unify the type system later...
list *
lmap(list *l, closure *cl)
```

```c
//yup, and filter too
list *
filter(list *l, bool (*fn)(void *, void *), void *args)
```

```c
//for fun
list *
range(int start, int end)
```

# Closures

Closures are built around two types: a closure and an environment variable

```c
struct closure {
  void *(*fn)(list *);
  list *env;
};

struct envobj {
  void *val;
  ssize_t size;
};
```

A closure is a function that is bound to an environment.  To bind an environment variable to a closure, use the bind function.

```c
closure *
bind(closure *c, void *(*fn)(list *), envobj *env);
```

To call this closed function, we use the call function:

```c
void *
call(closure *c, envobj *env);
```

To make environment variables, we need to lift our types into the environment.  

```c
//returns a lifted integer
envobj *
liftint(int a);
```

```c
//this transforms a list into an environment
list *
liftlist(list *l, ssize_t s) 
```

# Garbage Collector

The garbage collector is defined is gc.c . It maintains a linked list of every reference you register with it.  On a call to gc_collect, it frees the references based on the handlers that you have specified.

```c
//this starts the garbage collector.  You should call this before you do anything else
void gc_init(void);
```

Lists, environment objects, and closures are automatically registered with the garbage collector.

```c
//supported types
enum TYPE {
  LIST,
  ENVOBJ,  
  CLOSURE,
  STANDARD //gc's an generic obj   
};
```

To register any pointer:

```c
//type = STANDARD with for a generic pointer
void 
gc_register(void *obj, TYPE type);
```

To register a new type with the garbage collector, add it to enum TYPE (update gc.h), and then update gc_init to register your handler.

```c
void 
gc_register_destructor(TYPE, void (*)(void *));
```
A call to gc_collect performs garbage collection:

```c
void
gc_collect(void);
```

Other functions:

```c
//prints what the garbage collector is tracking
void 
gc_print(void);
```

Expirimental / Untested functionality:

```c
//tells the garbage collector to mark an object for safe keeping.
//it will now be tracked by the collector, but not freed until it is unmarked.
void 
gc_mark(void *obj);

//unmarks an object
void 
gc_unmark(void *obj);

//tells the garbage collector to stop tracking an object
void 
gc_remove(void *obj);
```

License
=======

Copyright (c) 2013 Charles Cary

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

