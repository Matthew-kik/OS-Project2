all: oss worker

oss: oss.o
	gcc -o oss oss.o

oss.o: oss.c
	gcc -Wall -c oss.c

worker: worker.o
	gcc -o worker worker.o

worker.o: worker.c
	gcc -Wall -c worker.c

clean:
	rm -f oss worker *.o
