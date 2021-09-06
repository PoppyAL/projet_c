#include <stdio.h>
#include <pthread.h> 
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#define CLE 217 //cle de la SHM  

int main() { 

	char commande[20];
	int status;
	const int nbuff = 100;
	
	int rep;
	//VAR MEMOIRE PARTAGEE
	char* mem; //pointuer sur la SHM
	int shmid; // id de la SHM

	FILE *f;
	char pid_screen[8];
	
	int fdsShell[2];
	pipe(fdsShell);

	//GET PID
	f = fopen("pid.txt", "r"); 		//r for Read
	if (f == NULL){
		printf("No such file !");
		return 0;
	}
	fscanf(f, "%s", pid_screen); 
	printf("PID : %s\n", pid_screen);
	fclose(f);	
	
	//CREATE FORK
	int noyau;
	noyau = fork();
	if (noyau != 0){ //PERE = SHELL
		printf("JE SUIS LE SHELL, BONJOUR !\n");
		char n;
		char res_received[nbuff];
		
		//CREATE SHM
		shmid = shmget((key_t)CLE, 1000, IPC_CREAT|0750);
		mem = shmat(shmid, NULL, 0);

		do {
			
			//SEND SIGNAL TO SCREEN
			rep = kill((int)pid_screen, SIGSTOP);
			printf("Rep kill : %d\n", rep);

			//ASK FOR COMMANDE			
			printf("Shell - Quelle est votre commande ? (\"stop\" pour fermer le terminal)\n");
			scanf("%s", &commande);	
			fflush(stdin);
			printf("Shell - Je suis le pere et j'envoie au shell : %s\n", commande);
				printf("1 %s", commande);

			if (*commande != "stop"){
				printf("2 %s", commande);
				//SEND COMMANDE TO NOYAU
				write(fdsShell[1], &commande, strlen(commande));

				//GET RESULT BACK FROM NOYAU
				n = read(fdsShell[0], res_received, nbuff-1);
				res_received[n] = '\0';
				printf("Command back with result : %s\n", res_received);

				//SEND TO SCREEN
				strcpy(mem, res_received);

				
			} else {
				printf("IL Y A EU UN STOOOP! 1");
				return (0);
			}

		} while (commande != "stop");
		printf("IL Y A EU UN STOOOP! 2");
		exit (0);
		//wait(&status);

	} else { //FILS = NOYAU
		char res_received[nbuff];
		char n;
		
		//READ IN PIPE
		n = read(fdsShell[0], res_received, nbuff-1);
		res_received[n] = '\0';
		printf("Shell - Je suis le fils et je recois : %s\n", res_received);
		
		//TRAITEMENT SGF

		//SEND RESULT IN PIPE
		write(fdsShell[1], &res_received, strlen(res_received));
	}

	return (0);
}
