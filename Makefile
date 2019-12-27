test : dining.o
	gcc -o dining dining.o -lpthread

dining.o : dining.c
	gcc -c dining.c
