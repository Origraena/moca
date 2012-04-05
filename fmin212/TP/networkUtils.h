
#ifndef DEF_FONC_SITE
#define DEF_FONC_SITE

#include <arpa/inet.h>
#include <errno.h>
#include <ifaddrs.h>
#include <math.h>
#include <netdb.h>
#include <net/ethernet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>

#include "utils.h"


#define PORT_SEND 31000
#define PORT_RECV 31001

#define FREE_NETWORK()	if(this_site.neighbours != NULL) free(this_site.neighbours);
#define CLEAN_NETWORK()	close(this_site.sdSend); \
						close(this_site.sdRecv); \
						FREE_NETWORK()


typedef struct site {
	in_addr_t broadcastAdd;
	int sdSend;
	int sdRecv;
	int running;
	struct sockaddr_in* neighbours;
	size_t nbNeighbours;
	struct sockaddr_in *neighboursTmp;
} site;

typedef enum site_state {
	IDLE = 1,
	WAITING = 2,
	WORKING = 3
} site_state;


// ??????? ou comment faire de la prog objet en C ^^
extern site this_site;


char* getIPaddress();
int init_network(int argc, char** argv);
int backupSocketNeighbours();
int recoverSocketNeighbours(struct sockaddr_in paramsNewNeighbour);
int hostsUpdate(struct sockaddr_in netParamsNeighbour);
int recvMessage(msg_t *message, struct sockaddr_in* add);

int broadcast(msg_t t);
int sendMessage(int siteID, msg_t msg);
int sendMessageWithAdd(msg_t msg);
void printNeighbours();
void getMessageFromString(char* string, msg_type* type, char** message);
int getNeighbour(unsigned long s_addr);
char *getIPstrFromNb(int nb);

#endif


