
#include "networkUtils.h"



char* getIPaddress() {
  char ac[80];
  if (gethostname(ac, sizeof(ac)) == -1) {
    perror("Error when getting local host name ");
    return NULL;
  }
//  printf("Nom de l'hôte : %s.\n", ac);


	struct ifaddrs *ifaddr, *ifa;
	int family, s;
	char host[NI_MAXHOST];
	char* res = NULL;

	if (getifaddrs(&ifaddr) == -1) {
		perror("getifaddrs");
		exit(EXIT_FAILURE);
	}

	/* Walk through linked list, maintaining head pointer so we
	 	 can free list later */
//	printf("Adresses IP disponibles :\n");
	for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
		if (ifa->ifa_addr == NULL)
			continue;

		family = ifa->ifa_addr->sa_family;

		/* For an AF_INET* interface address, display the address */

		if (family == AF_INET) {
			/* Display interface name and family (including symbolic
			 	 form of the latter for the common families) */
//			printf("%s", ifa->ifa_name);

			s = getnameinfo(ifa->ifa_addr,
					(family == AF_INET) ? sizeof(struct sockaddr_in) :
					sizeof(struct sockaddr_in6),
					host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
			if (s != 0) {
				printf("getnameinfo() failed: %s\n", gai_strerror(s));
				exit(EXIT_FAILURE);
			}
//			printf("\t<%s>\n", host);
			if(ifa->ifa_name[0] != 'l' || ifa->ifa_name[1] == 'o') {
				res = malloc(sizeof(char)*NI_MAXHOST);
				strcpy(res, host);
				this_site.broadcastAdd = (((((struct sockaddr_in*)(ifa->ifa_netmask))->sin_addr.s_addr) ^ (unsigned int)(pow(2,32)-1)) | inet_addr(res));
			}
		}
	}

//  printf("Adresse choisie : %s\n", res);
//	printf("Adresse de broadcast : %s\n", inet_ntoa(*(struct in_addr *)&this_site.broadcastAdd));
	freeifaddrs(ifaddr);
	return res;
}


int init_network(int argc, char* argv[]) {
	int enabled = 1;

	/* sockets initialisation */
	this_site.sdSend = socket(AF_INET, SOCK_DGRAM, 0);
	if(this_site.sdSend < 0) {
		perror("Erreur d'ouverture de socket ");
		return -1;
	}
	setsockopt(this_site.sdSend, SOL_SOCKET, SO_BROADCAST, &enabled, sizeof(enabled));

	this_site.sdRecv = socket(AF_INET, SOCK_DGRAM, 0);
	if(this_site.sdRecv < 0) {
		perror("Erreur d'ouverture de socket ");
		return -1;
	}
	setsockopt(this_site.sdRecv, SOL_SOCKET, SO_BROADCAST, &enabled, sizeof(enabled));

	/* sockets binding */
	char* addr = getIPaddress();
	struct sockaddr_in myNetParams;
	bzero(&myNetParams,sizeof(myNetParams));
	myNetParams.sin_family = AF_INET;
	myNetParams.sin_addr.s_addr = inet_addr(addr);
	myNetParams.sin_port = htons(PORT_SEND);
	free(addr);
	if(bind(this_site.sdSend, (struct sockaddr*)&myNetParams, (socklen_t)sizeof(myNetParams)) == -1) {
		perror("Erreur de lien a la boite reseau ");
		return -1;
	}

	myNetParams.sin_family = AF_INET;
	myNetParams.sin_addr.s_addr = INADDR_ANY;
	myNetParams.sin_port = htons(PORT_RECV);
	if(bind(this_site.sdRecv, (struct sockaddr*)&myNetParams, (socklen_t)sizeof(myNetParams)) == -1) {
		perror("Erreur de lien a la boite reseau ");
		return -1;
	}

	/* this_site variables init */
	this_site.neighbours = NULL;
	this_site.neighboursTmp = NULL;
	this_site.nbNeighbours = 0;
	this_site.running = 1;

//	printf("Lancement du site\n");

	//printf("Attente des autres sites…\n");
	//return waitForHellorep(5);

	return 0;
}

int backupSocketNeighbours() {
	int j;
	this_site.neighboursTmp = malloc(sizeof(struct sockaddr_in)*this_site.nbNeighbours);
	if(this_site.neighboursTmp == NULL) {
		perror("Erreur d'allocation ");
		return -1;
	}
	for(j = 0 ; j < this_site.nbNeighbours ; j++)
		this_site.neighboursTmp[j] = this_site.neighbours[j];

	return 0;
}

int recoverSocketNeighbours(struct sockaddr_in paramsNewNeighbour) {
	int j;
	this_site.neighbours = realloc(this_site.neighbours, sizeof(struct sockaddr_in)*this_site.nbNeighbours);
	if(this_site.neighbours == NULL) {
		perror("Erreur de reallocation ");
		return -1;
	}

	bzero(&((this_site.neighbours)[this_site.nbNeighbours-1]),sizeof((this_site.neighbours)[this_site.nbNeighbours-1]));
	((this_site.neighbours)[this_site.nbNeighbours-1]).sin_family = paramsNewNeighbour.sin_family;
	((this_site.neighbours)[this_site.nbNeighbours-1]).sin_port = paramsNewNeighbour.sin_port;
	((this_site.neighbours)[this_site.nbNeighbours-1]).sin_addr.s_addr = paramsNewNeighbour.sin_addr.s_addr;

	for(j = 0 ; j < this_site.nbNeighbours-1 ; j++)
		(this_site.neighbours)[j] = this_site.neighboursTmp[j];

	free(this_site.neighboursTmp);

	return 0;
}

int broadcast(msg_t message) {
	struct sockaddr_in netParamsNeighbour;
	bzero(&netParamsNeighbour,sizeof(netParamsNeighbour));
	netParamsNeighbour.sin_family = AF_INET;
	netParamsNeighbour.sin_port = htons(PORT_RECV);
	netParamsNeighbour.sin_addr.s_addr = this_site.broadcastAdd;

//	printf("Broadcast d'un message de type %d'\n", type(message));
	if (sendto(this_site.sdSend, (void *)&message, SIZE, 0, (struct sockaddr *)&netParamsNeighbour,sizeof(netParamsNeighbour)) == -1) {
		perror("sendto broadcast ");
		return -1;
	}

	return 0;
}

int sendMessage(int siteID, msg_t m) {
	//printf("Send message to num %d ; ", siteID);
	struct sockaddr_in netParamsNeighbour;
	bzero(&netParamsNeighbour,sizeof(netParamsNeighbour));
	netParamsNeighbour.sin_family = AF_INET;
	netParamsNeighbour.sin_port = htons(PORT_RECV);
	netParamsNeighbour.sin_addr.s_addr = this_site.neighbours[siteID].sin_addr.s_addr;
	//printf("addr neighbours[siteID] %ul ; ", this_site.neighbours[siteID].sin_addr.s_addr);
	//printf("addr envoyee %ul ; ", netParamsNeighbour.sin_addr.s_addr);


//	printf("Envoi d'un message de type %d a %s\n", type(m), inet_ntoa(this_site.neighbours[siteID].sin_addr));
	if (sendto(this_site.sdSend, (void *)&m, SIZE, 0, (struct sockaddr *)&netParamsNeighbour,sizeof(netParamsNeighbour)) == -1) {
		perror("sendto message ");
		return -1;
	}
	return 0;
}

int sendMessageWithAdd(msg_t m) {
	struct sockaddr_in netParamsNeighbour;
	bzero(&netParamsNeighbour,sizeof(netParamsNeighbour));
	netParamsNeighbour.sin_family = AF_INET;
	netParamsNeighbour.sin_port = htons(PORT_RECV);
	inet_aton(ip(m), &netParamsNeighbour.sin_addr);

//	printf("Envoi d'un message de type %d a %s\n", type(m), inet_ntoa(netParamsNeighbour.sin_addr));
	if (sendto(this_site.sdSend, &m, SIZE, 0, (struct sockaddr *)&netParamsNeighbour,sizeof(netParamsNeighbour)) == -1) {
		perror("sendto message ");
		return -1;
	}
	return 0;
}

int hostsUpdate(struct sockaddr_in netParamsNeighbour) {
	int indice = getNeighbour(netParamsNeighbour.sin_addr.s_addr);
	if(indice == -1) {
//		printf("Hote inconnu. ");
		// backup neighbours sockets
		if(backupSocketNeighbours() == -1)
			return -1;

		this_site.nbNeighbours++;

		// recover neighbours sockets
		if(recoverSocketNeighbours(netParamsNeighbour) == -1)
			return -1;
	}
//	else
//		printf("Hote connu. ");

	return 0;
}

void printNeighbours() {
	int i;
	printf("Neighbours : ");
	for(i = 0 ; i < this_site.nbNeighbours ; i++)
		printf("%s ", inet_ntoa(this_site.neighbours[i].sin_addr));

	printf("\n");
}

void getMessageFromString(char* string, msg_type* type, char** message) {
	*type = string[0]-48;
	*message = malloc(sizeof(char)*strlen(string));
	strcpy(*message, string+1);
}

int recvMessage(msg_t* message, struct sockaddr_in* add) {
	struct sockaddr_in netParamsNeighbour;
	bzero(&netParamsNeighbour,sizeof(netParamsNeighbour));
	size_t size = sizeof(netParamsNeighbour);

	int nbLus = recvfrom(this_site.sdRecv, message, SIZE, 0, (struct sockaddr *)&netParamsNeighbour, (socklen_t *)&size);
	if(nbLus != SIZE) {
		if(errno != EAGAIN) {
			fprintf(stderr, "erreur numero %d ; ", errno);
			perror("recvfrom ");
		}
		return -1;
	}

	strncpy(message->_sender, inet_ntoa(netParamsNeighbour.sin_addr), 16);

//	printf("Message recu depuis l'adresse %s et le port %d. ", message->_sender, ntohs(netParamsNeighbour.sin_port));

	if(hostsUpdate(netParamsNeighbour) == -1) 
		return -1;

	if(add != NULL) {
		bzero(add,sizeof(add));
		add->sin_family = AF_INET;
		add->sin_port = netParamsNeighbour.sin_port;
		add->sin_addr = netParamsNeighbour.sin_addr;
	}

//	printf("Message de type %u.\n", message->_typ_mes);

	return 0;
}

int getNeighbour(unsigned long s_addr) {
	int indice = -1, i;

	for(i = 0 ; i < this_site.nbNeighbours ; i++)
		if(s_addr == this_site.neighbours[i].sin_addr.s_addr) {
			indice = i;
//			printf ("%s\n", inet_ntoa(this_site.neighbours[i].sin_addr));
			break;
		}

	return indice;
}


// A revoir
char *getIPstrFromNb(int nb) {
	char *ipStr = (char *) malloc (IPLONG*sizeof(char));
	strncpy(ipStr,inet_ntoa(this_site.neighbours[nb].sin_addr), IPLONG);
	return ipStr;
}


