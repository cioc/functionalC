all: funtime

cflags = -c -std=c99

funtime: functional.o closure.o list.o main.o
	gcc functional.o closure.o list.o main.o -o test

closure.o: closure.c
	gcc $(cflags) closure.c -o closure.o

functional.o: functional.c
	gcc $(cflags) functional.c -o functional.o

list.o: list.c
	gcc $(cflags) list.c -o list.o

main.o: main.c
	gcc $(cflags) main.c -o main.o

clean:
	rm *.o test

