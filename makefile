all: main.o steup.o
	gcc main.o

run:
	./a.out $(args)

main.o: main.c
	gcc -c main.c

setup.o: setup.c
	gcc -c setup.c

clean:
	rm ./a.out
	rm *.o
