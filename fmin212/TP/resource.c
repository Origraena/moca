#define BIN_RESOURCE 1
#include <sys/select.h>
#include <signal.h>
#include <time.h>


#include "networkUtils.h"
#include "utils.h"
#include "partition.h"
#include "naimiTrehel.h"

#define CLEAN()	printf("CLEAN...\n"); \
				free_problem(this_problem); \
				CLEAN_NETWORK()

site this_site;
problem this_problem;

int _verbose = 1;
int pipeW,pipeR;
int pipeW2,pipeR2;

// {{{ Handler
/* ending handler. Necessary to clean the environnement. */
void end_handler(int sig) {
	printf("\nSignal caught\n");
	if(sig == 11) {
		printf("Segmentation fault\n");
		exit(EXIT_FAILURE);
	}
	
	this_site.running = 0;	
}
// }}}

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

	/* problem init */
	init_problem_resource(0);

	/* Execution loop */
//	print_help();

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
//		if(FD_ISSET(STDIN_FILENO, &socketRset))
//			standardInput();
			

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


