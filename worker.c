#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

const int BUFF_SZ = sizeof(int)*2; // Two ints to account for seconds and nanoseconds.

int main(int argc, char** argv) {

	int shm_id = shmget(859285, BUFF_SZ, 0666);
	if (shm_id <= 0 ) {
		fprintf(stderr, "Error in Child Shared Memory.\n");
		exit(1);
	}

// Shared memory pointer
int*shm_ptr = (int *)shmat(shm_id, NULL, 0);
if (shm_ptr == (void *)-1){
	fprintf(stderr, "Shared memory failed\n");
	exit(1);
}

int sec = shm_ptr[0]; //Seconds
int nano = shm_ptr[1]; //Nanoseconds



}
