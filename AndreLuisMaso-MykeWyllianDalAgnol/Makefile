All: a.out

a.out: main.c bigint.o llstack.o
	gcc -o a.out main.c bigint.o llstack.o

bigint.o: bigint.c
	gcc -c bigint.c

llstack.o: llstack.c
	gcc -c llstack.c

order:
	rm *.o *~
clean:
	rm *.o *~ a.out

