all: semaphone.o
	gcc semaphone.o

run:
	./a.out

semaphone.o:
	gcc -c semphone.c

clean:
	rm ./a.out
	rm *.o
