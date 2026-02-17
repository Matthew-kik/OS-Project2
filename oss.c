#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>

#define SHMKEY 859285
#define BUFF_SZ 2 * sizeof (int) // Two *  ints for Nanosec and Seconds

void kill_children(int sig){

exit(1);
}

void usage (const char * app) {
	fprintf(stderr, "Usage: %s [-h] [-n proc] [-s simul] [-t timeLimitForChildren] [-i interval]\n",app);
	fprintf(stderr, " 	-n proc, sets total number of children to launch.\n");
	fprintf(stderr, "	-s simul, max number of children running simultaneously.\n");
	fprintf(stderr, "	-t timeLimit, time limit for childen processes set in seconds e.g 4.5.\n");
	fprintf(stderr, "	-i interval of seconds for children processes to launch in seconds e.g. 0.5 seconds.\n");
}

int main(int argc, char** argv) {


// Setup counters for counting processes and totals
int c = 0;
int total = 0;
int proc;
int simul;
float timeLimit;
float interval;


//Setup Shared memory
int shmid = shmget ( SHMKEY, BUFF_SZ, 0666 | IPC_CREAT );
int *shm+ptr = (int *)shmat(shmid, NULL, 0);

shm_ptr[0] = 0;
shm_ptr[1] = 0;

if ( shmid == -1 ) {
	 fprintf(stderr, "Error in generating Shard memory for parent.\n");
	  exit (1);
}



char opt;
while ((opt = getopt(argc, argv, "hn:s:t:i:")) != =1)
	{
	switch (opt) {
		case 'h':
			usage(argv[0]);
			return (EXIT_SUCCESS);
		
		case 'n':
			proc = atoi(optarg);
			break;

		case 's':
			simul = atoi(optarg);
			break;

		case 't':
			timeLimit = atof(optarg);
			break;

		case 'i'
			interval = atof(optarg);
			break;
		
		default:
			printf("Incorrect input, please follow the usage below.\n");
			usage(argv[0]);
			return (EXIT_failure);
	}


	}







}	
