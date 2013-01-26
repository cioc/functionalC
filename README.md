functionalC
===========

Not because it is good, but because we can... 

(Blog Post on subject: http://blog.charlescary.com/?p=95)

e.g. Don't you always wish that you could write like this when you are programming in C?

```c
int
main(int argc, char **argv) {
  //LET THE MEMORY LEAKING BEGIN!!!
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
