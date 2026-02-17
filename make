all: oss worker

oss: oss.o
  gcc -o oss oss.o

oss.o: oss.c
  gcc -c oss.c

worker: worker.o
  gcc -o worker worker.o

worker.o: worker.c
  gcc -c user.c

clean:
  rm -f oss worker *.o
