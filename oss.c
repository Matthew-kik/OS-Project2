#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>

// Macro defined key and buffer size
#define SHMKEY 859285
#define BUFF_SZ 2 * sizeof (int) // Two *  ints for Nanosec and Seconds

//Process control table
struct PCB {

	int occupied;
	pid_t pid;
	int startSeconds;
	int startNano;
	int endingTimeSeconds;
	int endingTimeNano;

};


//Loops throuch process table and clears shared memory if 60 seconds have passed
void kill_processes(int sig){

	for(int i = 0; i < 20; i++){
		if (processTable[i].occupied){
			kill(processTable[i].pid, SIGTERM);
			}
	}	
// Cleans shared memory
shmdt(shm_ptr);
shmctl(shmid, IPC_RMID, NULL);
exit(1);

}

void usage (const char * app) {
	fprintf(stderr, "Usage: %s [-h] [-n proc] [-s simul] [-t timeLimitForChildren] [-i interval]\n",app);
	fprintf(stderr, " 	-n proc, sets total number of children to launch.\n");
	fprintf(stderr, "	-s simul, max number of children running simultaneously.\n");
	fprintf(stderr, "	-t timeLimit, time limit for childen processes set in seconds e.g 4.5.\n");
	fprintf(stderr, "	-i interval of seconds for children processes to launch in seconds e.g. 0.5 seconds.\n");
}

// Global variables 
struct PCB processTable[20];
int shmid;
int *shm_ptr;

int main(int argc, char** argv) {

// Signal to kill all children processes, then frees up memory after 60 seconds. 
signal(SIGALRM, kill_processes); // If 60 seconds pass
signal(SIGINT, kill_processes); // Handles freeing memory if ctrl-c is utilized, 
				// I didn't add SIGSTP with ctrl-z, as that seemed overly complex.
alarm(60);

// Setup counters for counting processes and totals
int c = 0;
int total = 0;
int proc;
int simul;
float timeLimit;
float interval;

//Setup Shared memory, error checking if no id is found
int shmid = shmget ( SHMKEY, BUFF_SZ, 0666 | IPC_CREAT );

if ( shmid == -1 ) {
         fprintf(stderr, "Error in generating Shard memory for parent.\n");
          exit (1);
}

// Intitializes pointer and attaches ID to whatever memory location the OS decides. 
int shm_ptr = (int *)shmat(shmid, NULL, 0);
shm_ptr[0] = 0; // Counter for seconds
shm_ptr[1] = 0; // Counter for nanoseconds 

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
