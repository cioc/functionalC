functionalC
===========

Not because it is good, but because we can...

e.g. Don't you always wish that you could write like this when you are programming in C?

```c
int
main(int argc, char **argv) {
  //LET THE MEMORY LEAKING BEGIN!!!
  //(you can probably guess what these do...)

  iter(map(range(0, 10), dbl, NULL), printint, NULL);
  iter(filter(range(0, 10), odd, NULL), printint, NULL); 
  exit(0);
}
```

No?  Neither did I.  This is horrible.  

Function C Programming Doc
==========================

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
//yup, and filter too
list *
filter(list *l, bool (*fn)(void *, void *), void *args)
```

```c
//for fun
list *
range(int start, int end)
```
