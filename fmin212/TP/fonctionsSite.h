
#include <arpa/inet.h>
#include <errno.h>
#include <ifaddrs.h>
#include <netdb.h>
#include <net/ethernet.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/wait.h>
#include <unistd.h>



#define PORT_SEND 31000
#define PORT_RECV 31001


int gstArgs(int argc, char* argv[], int* sdSend, int*sdRecv);
char* itoa(long n);
int backupSocketNeighbors(struct sockaddr_in* neighbors, struct sockaddr_in** neighborsTmp, int nbNeighbors);
int recoverSocketNeighbors(struct sockaddr_in** neighbors, struct sockaddr_in** neighborsTmp, int nbNeighbors, struct sockaddr_in paramsNewNeighbor);

int broadcast(char* message, int sdSend);


