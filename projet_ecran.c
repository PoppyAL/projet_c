#include <stdio.h>
#include <pthread.h> 
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>
#include <stdbool.h>

#define CLE 217 //cle de la SHM 
bool end = false;

void action_handler(int signal){
	printf("ACTION_HANDLER");
	if(signal == SIGSTOP)
		printf("SIGSTOP received !!\n");
	if(signal == SIGCONT)
		printf("SIGCONT received !! \n");
	if(signal == SIGHUP)
		printf("FERMETURE DU TERMINAL !!\n");
	if (signal == SIG_ERR)
		printf("ERROOOOOR!");
}
FILE * f;
int main() {
	pid_t pid = getpid();
	f = fopen("pid.txt", "r+");
	fprintf(f, "%d", getpid());
	printf("PID : %d", getpid());
	fclose(f);
	int rep;
	int rep2;
	
	printf("VOICI LE PLUS BEL ECRAN DU MONDE\n");
	sleep(1);
	//rep2 = kill(getpid(), SIGCONT);
	//printf("Rep2 kill : %d\n", rep2);
	char* mem; //pointeur sur la SHM
	do {	
		sleep(1);
		printf("1");
		pause();
		printf("2");
		struct sigaction action;
		action.sa_handler = action_handler;
		sigemptyset(&action.sa_mask);
		action.sa_flags = SA_RESTART;
		if (sigaction(SIGHUP, &action, NULL) == -1) {
			printf("impossible d'appeler sigaction stop\n");
		}
		
		int shmid = shmget((key_t)CLE, 0, 0); //récup id de la SHM
		mem = shmat(shmid, NULL, 0); // attachement à la SHM
		printf("Lu dans la SHM : %s\n", mem); // lecture de la SHM
	} while (strcmp(mem, "stop") != 0);

	return (0);
}
