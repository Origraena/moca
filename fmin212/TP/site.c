

#include <sys/select.h>
#include <signal.h>
#include <time.h>


#include "networkUtils.h"
#include "naimiTrehel.h"
#include "utils.h"

#define CLEAN()	printf("CLEAN...\n"); \
				CLEAN_NETWORK()

site this_site;


/* ending handler. Necessary to clean the environnement. */
void end_handler(int sig) {
	printf("\nSignal caught\n");
	if(sig == 11) {
		printf("Segmentation fault\n");
		exit(EXIT_FAILURE);
	}
	
	this_site.running = 0;	
}

void print_help() {
	printf("1. BROADCAST\n");
	printf("2. MESSAGE\n");
	printf("3. NEIGHBOURS\n");
	printf("4. CRITSECTION\n");
	printf("5. LAST\n");
	printf("6. NEXT\n");
	printf("7. TOKEN\n");
	printf("8. STATE\n");
}

void standardInput() {
	msg_t envoi;
	int rea;
	char adip[20];

	scanf("%d", &rea);
	printf("\n");

	switch(rea) {
		case 1:
			printf("BROADCAST.\n");

			type(envoi) = MESSAGE;
			broadcast (envoi);
			break;
		case 2:
			printf ("TO : ");
			int nbLus = read(STDIN_FILENO, adip, 15);
			if (nbLus < 1) {
				printf ("Error");
				break;
			}
			type(envoi) = MESSAGE;
			adip[15] = '\0';
			strncpy(ip(envoi), adip, 16);
			sendMessageWithAdd(envoi);

			printf ("Send Message\n");
			break;
		case 3:
			printNeighbours();
			break;
		case 4:
			critSectionRequest();
			break;
		case 5:
			printf("Last : %d ", last);
			if(last != -1)
				printf("; %s", inet_ntoa(*&this_site.neighbours[last].sin_addr));
			printf("\n");
			break;
		case 6:
			printf("Next : %d ", next);
			if(next != -1)
				printf("; %s", inet_ntoa(*&this_site.neighbours[next].sin_addr));
			printf("\n");
			break;
		case 7:
			if (tokenPresent)
				printf("Present\n");
			else 
				printf("Absent\n");
			break;
		case 8:
			printf("State %d\n", state);
			break;
		default:
			printf ("Invalid command\n");
			print_help();
			break;
	}
}

int main(int argc, char* argv[]) {
	fd_set socketRset;
	msg_t msg;
	
	/* Signals attachment to the handler */
	struct sigaction action;
	memset(&action, 0, sizeof(action));
	action.sa_handler = end_handler;
	if (sigaction(SIGPIPE,&action,NULL))
		perror ("sigaction ");
	if (sigaction(SIGINT,&action,NULL))
		perror ("sigaction ");
	if (sigaction(11,&action,NULL)){
		perror ("sigaction");
		exit(EXIT_FAILURE);
	}
	
	/* network initialisation */
	if(init_network(argc, argv) == -1) {
		CLEAN()
		exit(EXIT_FAILURE);
	}
	
	/* Naimi-Trehel structures init */
	init_structures();
	
	/* Execution loop */
	print_help();

	while(this_site.running) {
		/* select settings */
		FD_ZERO(&socketRset);
		FD_SET(STDIN_FILENO, &socketRset);
		FD_SET(this_site.sdRecv, &socketRset);
		
		
		/* select on all reading descriptors */
		if(select(this_site.sdRecv+1, &socketRset, NULL, NULL, NULL) == -1) {
			if (errno == EINTR)
				continue;
			perror("select ");
			CLEAN()
			exit(EXIT_FAILURE);
		}
		
		
		/* on standard input */
		if(FD_ISSET(STDIN_FILENO, &socketRset))
			standardInput();
		
		
		/* on message reception */
		if(FD_ISSET(this_site.sdRecv, &socketRset)) {
			if(recvMessage(&msg, NULL) == -1) {
				CLEAN()
				exit(EXIT_FAILURE);
			}
			
			if(type(msg) == MESSAGE) {}
			else
				handleMessage(msg);
		}
	}
	
	CLEAN()
	printf("Fermeture du site.\n");
	
	return 0;
}


