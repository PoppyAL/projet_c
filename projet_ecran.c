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

void sig_handler(int signal){
	if(signal == SIGSTOP)
		printf("SIGSTOP received !!\n");
	if(signal == SIGHUP)
		printf("FERMETURE DU TERMINAL !!\n");
		//end = true;
		//return (0);
}

int main() {
	int rep;
	int rep2;
	
	printf("VOICI LE PLUS BEL ECRAN DU MONDE\n");
	//rep = kill(getpid(), SIGSTOP);
	//printf("Rep kill : %d\n", rep);
	sleep(5);
	printf("soon stop");
	rep2 = kill(getpid(), SIGCONT);
	printf("Rep2 kill : %d\n", rep2);
	
	while (1) {
		sleep(1);
		printf("-");
		if (signal(SIGSTOP, sig_handler) == SIG_ERR){
			printf("Can't catch SIGSTOP\n");
		}
		char* mem; //pointeur sur la SHM
		int shmid = shmget((key_t)CLE, 0, 0); //récup id de la SHM
		mem = shmat(shmid, NULL, 0); // attachement à la SHM
		printf("Lu dans la SHM : %s\n", mem); // lecture de la SHM
	};

	return (0);
}
