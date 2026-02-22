#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
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
int c = 0; // Counts children
int total = 0; // total launched children
int proc;
int simul;
float timeLimit;
float interval;

//Setup Shared memory, error checking if no id is found

shmid = shmget ( SHMKEY, BUFF_SZ, 0666 | IPC_CREAT );

if ( shmid == -1 ) {
         fprintf(stderr, "Error in generating Shard memory for parent.\n");
          exit (1);
}

// Intitializes pointer and attaches ID to whatever memory location the OS decides. 
shm_ptr = (int *)shmat(shmid, NULL, 0);
shm_ptr[0] = 0; // Counter for seconds
shm_ptr[1] = 0; // Counter for nanoseconds 

char opt;
while ((opt = getopt(argc, argv, "hn:s:t:i:")) != -1)
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

		case 'i':
			interval = atof(optarg);
			break;
		
		default:
			printf("Incorrect input, please follow the usage below.\n");
			usage(argv[0]);
			return (EXIT_FAILURE);
	}
        
	}

	// Declarations for status, pid, and counter for 0.5 seconds
	int status;
	pid_t pid;
	float lastPrintTime = 0;
	float currentTime = 0;
	
	
	while ( total  < proc || c > 0 ) {
	
	shm_ptr[1] += 10000000;	//10 miliseconds for counting		 
	if (shm_ptr[1] >= 1000000000){ 
		shm_ptr[0]++; 
		shm_ptr[1] -= 1000000000;
	}

	currentTime = shm_ptr[0] + (shm_ptr[1] / 1000000000.0);

	// Non-blocking wait call checking child termination
	pid = waitpid(-1, &status, WNOHANG);
	if (pid > 0){
		
		// Loops and counts process tables to check for terminated pids
		for (int i = 0; i < 20; i++) {
			if (processTable[i].pid == pid) {
			    processTable[i].occupied = 0;
			    c--;
			    break;
			}
		}
		//update
		//set entry to unoccupied

	}
	
	
	if (currentTime - lastPrintTime >= 0.5) {
	   
				
		lastPrintTime = currentTIme;
	}
	
	// Counts time before last launched
	float lastLaunchTime = 0;
										
	currentTime = shm_ptr[0] + (shm_ptr[1] / 1000000000.0);	// Calculates current time
	// Launch new child is conditions are met. 
	if ( c < simul && total < proc && (currentTime - lastLaunchTime) >= interval) {
	
	char secArg[20];
	char nanoArg[20];
	
	int workerSec = (int)timeLimit;
	int workerNano = (timeLimit - workerSec) * 1000000000;
	
	sprintf(secArg, "%d", workerSec);
	sprintf(nanoArg, "%d", workerNano);
	pid_t worker = fork();
	

	if ( worker == 0) {
	
		execl("./worker", "worker", secArg, nanoArg, NULL);
		fprintf(stderr, "Excel Failed\n");
		exit(1);
	}
	
	if (worker > 0) {

		// Fills PCB 
		for (int i = 0; i < 20; i++) {
			if (!processTable[i].occupied) {
				processTable[i].occupied = 1;
				processTable[i].pid = worker;
				processTable[i].startSeconds = shm_ptr[0];
				processTable[i].startNano = shm_ptr[1];
				break;
					}
				}
		//fork and exec 
		//fill PCB entry with pid, start time, and ending time
		lastLaunchTime = currentTime;
		c++;
		total++;
			}	
		}

	}

//print final report

 return(EXIT_SUCCESS);
}




