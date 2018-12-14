all: main.o setup.out
	gcc main.o

run: a.out
	./a.out

setup: setup.out
	./setup.out $(args)

setup.out: setup.o
	gcc -o setup.out setup.o

main.o: main.c
	gcc -c main.c

setup.o: setup.c
	gcc -c setup.c

clean:
	rm *.out
	rm *.o
