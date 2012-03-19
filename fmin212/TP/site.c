

#include "fonctionsSite.h"

#define FREE()	free(this_site.neighbours);
#define CLEAN()	close(this_site.sdSend); \
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
	char recit[1024];
	int i = 1, nbLus, size = sizeof(struct sockaddr_in);
	int known;
	fd_set socketRset;
	message m = {MESSAGE, NULL};
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
	if(gstArgs(argc, argv) == -1)
	{
		CLEAN()
		exit(EXIT_FAILURE);
	}
	
	/* HELLO message broadcasting */
	m.type = MESSAGE;
	m.content = malloc(sizeof(char)*1024);
	strcpy(m.content, "HELLO");
	printf("Broadcast HELLO\n");
	if(broadcast(&m) == -1)
	{
		free(m.content);
		CLEAN()
		exit(EXIT_FAILURE);
	}
	free(m.content);
	
	
	/* Execution loop */
	while(this_site.running)
	{
		/* select settings */
		FD_ZERO(&socketRset);
		FD_SET(STDIN_FILENO, &socketRset);
		FD_SET(this_site.sdRecv, &socketRset);
		
		
		/* select on all reading descriptors */
		if(select((this_site.sdSend > this_site.sdRecv ? this_site.sdSend : this_site.sdRecv)+1, &socketRset, NULL, NULL, NULL) == -1)
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
			m.content = malloc(sizeof(char)*1024);
			size = sizeof(struct sockaddr_in);
			nbLus = recvfrom(this_site.sdRecv, &m, 1023, 0, (struct sockaddr *)&netParamsNeighbour, (socklen_t *)&size);
			if(nbLus == -1)
			{
				perror("recvfrom ");
			}
			else
			{
				//m.content[nbLus-sizeof(int)] = 0;
			}
			
			printf("Message recu depuis l'adresse %s et le port %d. ", inet_ntoa(netParamsNeighbour.sin_addr), ntohs(netParamsNeighbour.sin_port));
			
			if(hostsUpdate(netParamsNeighbour) == -1)
			{
				free(m.content);
				CLEAN()
				exit(EXIT_FAILURE);
			}
			
			if(m.type == MESSAGE) /* on MESSAGE reception */
			{
				printf("Message recu : %s.\n", m.content);
				
				if(strcmp(m.content, "HELLO") == 0)
				{
					m.type = MESSAGE;
					strcpy(m.content, "HELLOREP");
					printf("Broadcast HELLOREP\n");
					if(broadcast(&m) == -1)
					{
						free(m.content);
						CLEAN()
						exit(EXIT_FAILURE);
					}
				}
			}
			else if(m.type == REQUEST) /* on REQUEST reception */
			{
				requestTreatment();
			}
			else
			{
				fprintf(stderr, "Probleme de reception du message.\n");
			}
			free(m.content);
		}
	}
	
	CLEAN()
	printf("Fermeture du site.\n");
	
	return 0;
}


