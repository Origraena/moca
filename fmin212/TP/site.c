

#include "fonctionsSite.h"

#define FREE()	if(this_site.neighbours != NULL) free(this_site.neighbours);
#define CLEAN()	printf("CLEAN...\n"); \
				close(this_site.sdSend); \
				close(this_site.sdRecv); \
				FREE()

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

int main(int argc, char* argv[])
{
	msg_type t;
	int nbLus, size = sizeof(struct sockaddr_in);
	fd_set socketRset;
	struct sockaddr_in netParamsNeighbour;
	bzero(&netParamsNeighbour,sizeof(netParamsNeighbour));
	
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
	
	/* Setting network parameters */
	if(init(argc, argv) == -1)
	{
		CLEAN()
		exit(EXIT_FAILURE);
	}
	
	/* HELLO message broadcasting */
	t = MESSAGE;
	printf("Broadcast HELLO\n");
	if(broadcast(t, "HELLO") == -1)
	{
		CLEAN()
		exit(EXIT_FAILURE);
	}
	
	
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
			// receive message
			size = sizeof(netParamsNeighbour);
			char recit[1024];
			nbLus = recvfrom(this_site.sdRecv, recit, (size_t)1023, 0, (struct sockaddr *)&netParamsNeighbour, (socklen_t *)&size);
			if(nbLus < 1)
			{
				perror("recvfrom ");
			}
			else
			{
				recit[nbLus] = 0;
			}
			
			char* msg;
			getMessageFromString(recit, &t, &msg);
			
			printf("Message recu depuis l'adresse %s et le port %d. ", inet_ntoa(netParamsNeighbour.sin_addr), ntohs(netParamsNeighbour.sin_port));
			
			
			if(hostsUpdate(netParamsNeighbour) == -1)
			{
				free(msg);
				CLEAN()
				exit(EXIT_FAILURE);
			}
			
			if(t == MESSAGE)
			{
				printf("Message recu : %s.\n", msg);
				
				if(strcmp(msg, "HELLO") == 0)
				{
					printf("Broadcast HELLOREP\n");
					if(broadcast(t, "HELLOREP") == -1)
					{
						free(msg);
						CLEAN()
						exit(EXIT_FAILURE);
					}
				}
			}
			else if(t == REQUEST)
			{
				requestTreatment();
			}
			else
			{
				fprintf(stderr, "Probleme de reception du message.\n");
			}
			free(msg);
		}
	}
	
	CLEAN()
	printf("Fermeture du site.\n");
	
	return 0;
}


