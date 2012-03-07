

#include "fonctionsSite.h"

#define FREE()	free(netParamsNeighbors);
#define CLEAN()	if(close(sdSend) == -1){perror("Erreur de close ");} \
				if(close(sdRecv) == -1){perror("Erreur de close ");} \
				FREE()

struct sockaddr_in* netParamsNeighbors = NULL;
int sdSend, sdRecv, continueB = 1;

/* ending handler. Necessary to clean the environnement. */
void end_handler(int sig)
{
	printf("\nSignal cought\n");
	if(sig == 11)
		printf("Segmentation fault\n");
	
	/* TODO */
	
	
	continueB = 0;
}

int main(int argc, char* argv[])
{
	char recit[1024];
	int i = 1, nbLus, nbNeighbors = 0, size = sizeof(struct sockaddr_in);
	int known;
	fd_set socketRset;
	struct sockaddr_in *neighborsTmp = NULL;
	struct sockaddr_in netParamsNeighbor;
	bzero(&netParamsNeighbor,sizeof(netParamsNeighbor));
	
	/* Signals attachment to the handler */
	struct sigaction action;
	memset(&action, 0, sizeof(action));
	action.sa_handler = end_handler;
	/*for(i = 0 ; i < 32 ; i++)
		if (sigaction(i,&action,NULL)){
			perror ("sigaction ");
		}*/
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
	if(gstArgs(argc, argv, &sdSend, &sdRecv) == -1)
	{
		CLEAN()
		exit(EXIT_FAILURE);
	}
	
	/* HELLO message broadcasting */
	printf("Broadcast HELLO\n");
	if(broadcast("HELLO", sdSend) == -1)
	{
		CLEAN()
		exit(EXIT_FAILURE);
	}
	
	
	/* Execution loop */
	while(continueB)
	{
		/* select settings */
		FD_ZERO(&socketRset);
		FD_SET(STDIN_FILENO, &socketRset);
		FD_SET(sdRecv, &socketRset);
		
		
		/* select on all reading descriptors */
		if(select((sdSend > sdRecv ? sdSend : sdRecv)+1, &socketRset, NULL, NULL, NULL) == -1)
		{
			if (errno == EINTR)
				continue;
			perror("select ");
			CLEAN()
			exit(EXIT_FAILURE);
		}
		
		printf("sortie select\n");
		
		
		/* on standard input */
		if(FD_ISSET(STDIN_FILENO, &socketRset))
		{
			printf("Entree standard\n");
			char buf[10];
			nbLus = read(STDIN_FILENO, buf, 9);
			buf[nbLus] = 0;
			if(nbLus < 1)
				perror("Erreur de reception sur l'entree standard");
			
			/* TODO */
		}
		
		
		/* on request reception */
		if(FD_ISSET(sdRecv, &socketRset))
		{
			// receive message
			nbLus = recvfrom(sdRecv, (void*)recit, 1023, 0, (struct sockaddr *)&netParamsNeighbor, (socklen_t *)&size);
			if(nbLus == -1)
			{
				perror("recvfrom ");
			}
			else
			{
				recit[nbLus] = 0;
			}
			
			printf("Message recu depuis l'adresse %s et le port %d\n", inet_ntoa(netParamsNeighbor.sin_addr), ntohs(netParamsNeighbor.sin_port));
			
			// message emmited toward the scanned port
			
			known = 0;
			for(i = 0 ; i < nbNeighbors ; i++)
			{
				if(ntohs(netParamsNeighbor.sin_addr.s_addr) == ntohs(netParamsNeighbors[i].sin_addr.s_addr))
				{
					known = 1;
				}
			}
			if(known == 0)
			{
				printf("Hote inconnu.\n");
				// backup neighbors sockets
				if(backupSocketNeighbors(netParamsNeighbors, &neighborsTmp, nbNeighbors) == -1)
				{
					CLEAN()
					exit(EXIT_FAILURE);
				}
				
				nbNeighbors++;
				
				// recover neighbors sockets
				if(recoverSocketNeighbors(&netParamsNeighbors, &neighborsTmp, nbNeighbors, netParamsNeighbor) == -1)
				{
					CLEAN()
					exit(EXIT_FAILURE);
				}
			}
			else
			{
				printf("Hote connu.\n");
			}
			
			
			if(strcmp(recit, "HELLO") == 0)
			{
				printf("Broadcast HELLOREP\n");
				if(broadcast("HELLOREP", sdSend) == -1)
				{
					CLEAN()
					exit(EXIT_FAILURE);
				}
				
			}	
				/* TODO traitement request*/
		}
	}
	
	CLEAN()
	printf("Fermeture du site.\n");
	
	return 0;
}


