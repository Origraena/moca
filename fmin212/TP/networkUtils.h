
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

extern site this_site;


char* getIPaddress();
int init_network(int argc, char** argv);
int backupSocketNeighbours();
int recoverSocketNeighbours(struct sockaddr_in paramsNewNeighbour);
int hostsUpdate(struct sockaddr_in netParamsNeighbour);
int recvMessage(msg_type* type, char** message);

int broadcast(msg_type t, char* msg);
int sendMessage(int siteID, msg_type t, char* msg);
int sendMessageWithAdd(char* add, msg_type t, char* msg);
void printNeighbours();
void getMessageFromString(char* string, msg_type* type, char** message);
int getNeighbour(unsigned long s_addr);

#endif


