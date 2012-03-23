

#include <sys/select.h>
#include <signal.h>


#include "networkUtils.h"
#include "naimiTrehel.h"
#include "utils.h"

#define CLEAN()	printf("CLEAN...\n"); \
				CLEAN_NETWORK()

site this_site;


/* ending handler. Necessary to clean the environnement. */
void end_handler(int sig)
{
	printf("\nSignal cought\n");
	if(sig == 11)
	{
		printf("Segmentation fault\n");
		exit(EXIT_FAILURE);
	}
	
	this_site.running = 0;	
}

void standardInput()
{
	int nbLus;
	char buf[50];
	msg_type t = MESSAGE;
	nbLus = read(STDIN_FILENO, buf, 49);
	if(nbLus < 1)
		perror("Erreur de reception sur l'entree standard");
	else
		buf[nbLus-1] = 0;
	
	
	if(strstr(buf, "BROADCAST") != NULL)
	{
		if((strlen(buf) == strlen("BROADCAST"))
		   ||
		   (strlen(buf) == strlen("BROADCAST")+1))
		{
			printf("Entrez le message : \n");
			nbLus = read(STDIN_FILENO, buf, 49);
			if(nbLus < 1)
				perror("Erreur de reception sur l'entree standard");
			else
				buf[nbLus-1] = 0;
			
			broadcast(t, buf);
		}
		else
		{
			broadcast(t, buf+strlen("BROADCAST")+1);
		}
	}
	else if(strstr(buf, "MESSAGE") != NULL)
	{
		printf("TO : \n");
		nbLus = read(STDIN_FILENO, buf, 49);
		if(nbLus < 1)
			perror("Erreur de reception sur l'entree standard");
		else
			buf[nbLus-1] = 0;
		
		char add[256];
		strcpy(add, buf);
		
		printf("CORPS DU TEXTE : \n");
		nbLus = read(STDIN_FILENO, buf, 49);
		if(nbLus < 1)
			perror("Erreur de reception sur l'entree standard");
		else
			buf[nbLus-1] = 0;
		
		sendMessageWithAdd(add, t, buf);
	}
	else if(strcmp(buf, "NEIGHBOURS") == 0)
	{
		printNeighbours();
	}
	else if(strcmp(buf, "CRITSECTION") == 0)
	{
		critSectionRequest();
	}
	else if(strcmp(buf, "LAST") == 0)
	{
		printf("Last : %d ", last);
		if(last != -1)
			printf("; %s", inet_ntoa(*&this_site.neighbours[last].sin_addr));
		printf("\n");
	}
	else if(strcmp(buf, "NEXT") == 0)
	{
		printf("Next : %d ", next);
		if(next != -1)
			printf("; %s", inet_ntoa(*&this_site.neighbours[next].sin_addr));
		printf("\n");
	}
	else if(strcmp(buf, "TOKEN") == 0)
	{
		if(tokenPresent)
			printf("present\n");
		else
			printf("absent\n");
	}
	else if(strcmp(buf, "TOKEN") == 0)
	{
		printf("state %d\n", state);
	}
	else
	{
		printf("Entree standard recue non traitee.\n");
	}
}

int main(int argc, char* argv[])
{
	msg_type t;
	char* msg;
	fd_set socketRset;
	
	/* Signals attachment to the handler */
	struct sigaction action;
	memset(&action, 0, sizeof(action));
	action.sa_handler = end_handler;
	if (sigaction(SIGPIPE,&action,NULL)){
		perror ("sigaction ");
	}
	if (sigaction(SIGINT,&action,NULL)){
		perror ("sigaction ");
	}
	if (sigaction(11,&action,NULL)){
		perror ("sigaction");
		exit(EXIT_FAILURE);
	}
	
	/* network initialisation */
	if(init_network(argc, argv) == -1)
	{
		CLEAN()
		exit(EXIT_FAILURE);
	}
	
	/* Naimi-Trehel structures init */
	init_structures();
	
	/* Execution loop */
	while(this_site.running)
	{
		/* select settings */
		FD_ZERO(&socketRset);
		FD_SET(STDIN_FILENO, &socketRset);
		FD_SET(this_site.sdRecv, &socketRset);
		
		
		/* select on all reading descriptors */
		if(select(this_site.sdRecv+1, &socketRset, NULL, NULL, NULL) == -1)
		{
			if (errno == EINTR)
				continue;
			perror("select ");
			CLEAN()
			exit(EXIT_FAILURE);
		}
		
		
		/* on standard input */
		if(FD_ISSET(STDIN_FILENO, &socketRset))
		{
			standardInput();
		}
		
		
		/* on message reception */
		if(FD_ISSET(this_site.sdRecv, &socketRset))
		{
			if(recvMessage(&t, &msg) == -1)
			{
				free(msg);
				CLEAN()
				exit(EXIT_FAILURE);
			}
			
			if(t == MESSAGE)
			{
				printf("Message recu : %s.\n", msg);
			}
			else
			{
				handleMessage(t, msg);
			}
			free(msg);
		}
	}
	
	CLEAN()
	printf("Fermeture du site.\n");
	
	return 0;
}


