

#include <sys/select.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>


#include "networkUtils.h"
#include "naimiTrehel.h"
#include "utils.h"
#include "partition.h"

#define CLEAN()	printf("CLEAN...\n"); \
				free_problem(this_problem); \
				CLEAN_NETWORK()

site this_site;
problem this_problem;
int _verbose = 0;

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

// {{{ print_help
void print_help() {
	printf("1. BROADCAST\n");
	printf("2. MESSAGE\n");
	printf("3. NEIGHBOURS\n");
	printf("4. CRITSECTION\n");
	printf("5. LAST\n");
	printf("6. NEXT\n");
	printf("7. TOKEN\n");
	printf("8. STATE\n");
	printf("9. PREDEC\n");
	printf("10. PANNE\n");
}
// }}}

// {{{ standardInput
// Lecture de l'entrée standard
void standardInput() {
	msg_t envoi;
	int rea;
//	int a = 0;
	int i = 0;
	char adip[20];

	scanf("%d", &rea);
	printf("\n");

	switch(rea) {
		case 1:
			printf("Verbose\n");
			if (_verbose) {
				_verbose = 0;
				printf("verbose mode desactivated...");
			}
			else {
				_verbose = 1;
				printf("verbose mode activated!");
			}
			break;
		case 2:
			printf ("Processing thread id = %i\n",this_problem.thread_id);
			break;
		case 3:
			printNeighbours();
			break;
		case 4:
			printf("Resource : %s\n",this_site.resource);
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
		case 9:
			for (i=0; i<=TOLERANCE; printf("%s - ", inet_ntoa(predec[i].sin_addr)), i++);
			printf("\n");
			break;
		case 10:
			if (ch_pid)
				pthread_kill ((pthread_t)ch_pid, SIGKILL);
			this_site.running = 0;
			break;
		default:
			printf ("Invalid command\n");
			print_help();
			break;
	}
}

int pipeW,pipeR,pipeW2,pipeR2;

int main(int argc, char* argv[]) {
	long seed = time(0);
	srand(seed);
	printf("Seed = %li\n",seed);


//	uint tmpW;
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

	init_structures();

	/* problem init */
	init_problem(0);

	/* Execution loop */
	print_help();
	int pipefd[2];
	if (pipe(pipefd) == -1) {
		perror("pipe creation error");
		CLEAN();
		exit(EXIT_FAILURE);
	}
	pipeR = pipefd[0];
	pipeW = pipefd[1];
	if (pipe(pipefd) == -1) {
		perror("pipe creation error");
		CLEAN();
		exit(EXIT_FAILURE);
	}
	pipeR2 = pipefd[0];
	pipeW2 = pipefd[1];


	if(pthread_create(&this_problem.thread_id, NULL, (void*)(processingThreadFunction),0) != 0) {
		fprintf(stderr, "Thread creation failure.\n");
		CLEAN();
		exit(EXIT_FAILURE);
	}


	while(this_site.running) {
		/* select settings */
		FD_ZERO(&socketRset);
		FD_SET(STDIN_FILENO, &socketRset);
		FD_SET(this_site.sdRecv, &socketRset);
		FD_SET(pipeR,&socketRset);
		FD_SET(pipeR2,&socketRset);
		

		int S = ((this_site.sdRecv > pipeW) && (this_site.sdRecv > pipeR)) ? this_site.sdRecv : (pipeR2 > pipeR) ? pipeR2 : pipeR;
		/* select on all reading descriptors */
		if(select(++S, &socketRset, NULL, NULL, NULL) == -1) {
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
		if(FD_ISSET(this_site.sdRecv, &socketRset)) {
			if(recvMessage(&msg, NULL) == -1) {
				CLEAN()
				exit(EXIT_FAILURE);
			}
			
			if((type(msg) == MESSAGE) || (type(msg) == SOLUTION)) {}
			else
				handleMessage(msg);
		}

		if (FD_ISSET(pipeR,&socketRset)) {
			char buf[2];
			read(pipeR,buf,sizeof(char)+sizeof(char));
			if (this_problem.processed) {
				if (!this_problem.sent) {
					if (strcmp(this_site.resource,"none") != 0) {
						if (this_problem.thread_id) {
							if (_verbose) printf("Join thread...\n");
							pthread_join(this_problem.thread_id,0);
							this_problem.thread_id = 0;
							if (_verbose) printf("Join thread!\n");
						}
						if (_verbose) printf("Requesting critical section...\n");
						this_problem.sent = 1;
						critSectionRequest();
					}
					else {
						if (_verbose)
								printf("No resource site !\n");
						CLEAN()
						exit(EXIT_FAILURE);
					}
				}
				else {
					if (_verbose)
							printf("Sent = %i\n",this_problem.sent);
				}
			}
			else {
					if (_verbose)
							printf("Processed = %i\n",this_problem.processed);
					//buf[0] = 'c';
					//buf[1] = '\0';
					//write(pipeW2,buf,sizeof(char)*2);
				}
		}

		if (FD_ISSET(pipeR2,&socketRset)) {
			char buf[2];
				read(pipeR2,buf,sizeof(char)*2);
			if (!this_problem.thread_id) {
				if(pthread_create(&this_problem.thread_id, NULL, (void*)(processingThreadFunction),0) != 0) {
					fprintf(stderr, "Thread creation failure.\n");
					CLEAN();
					exit(EXIT_FAILURE);
				}
//				pthread_detach(this_problem.thread_id);
			}
		}

	}
	
	CLEAN()
		pthread_mutex_destroy(&mut_check);
	printf("Fermeture du site.\n");
	
	return 0;
}


