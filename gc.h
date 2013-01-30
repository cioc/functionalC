#ifndef GC_H
#define GC_H
typedef enum TYPE TYPE;

enum TYPE {
  LIST,
  ENVOBJ,  
  CLOSURE,
  STANDARD //gc's an generic obj   
};

#define TYPE_COUNT 4

void gc_mark(void *obj);
void gc_unmark(void *obj);
void gc_register(void *obj, TYPE type);
void gc_remove(void *obj);
void gc_init(void);
void gc_collect(void);
void gc_print(void);
#endif
