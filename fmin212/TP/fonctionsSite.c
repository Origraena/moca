
#include "fonctionsSite.h"

char* getIPaddress()
{
    char ac[80];
    if (gethostname(ac, sizeof(ac)) == -1)
	{
        perror("Error when getting local host name ");
        return NULL;
    }
    printf("Nom de l'hôte : %s.\n", ac);
	
	
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
	printf("Adresses IP disponibles :\n");
	for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
		if (ifa->ifa_addr == NULL)
			continue;
		
		family = ifa->ifa_addr->sa_family;
		
		/* For an AF_INET* interface address, display the address */
		
		if (family == AF_INET)
		{
			/* Display interface name and family (including symbolic
			 form of the latter for the common families) */
			printf("%s", ifa->ifa_name);
			
			s = getnameinfo(ifa->ifa_addr,
							(family == AF_INET) ? sizeof(struct sockaddr_in) :
							sizeof(struct sockaddr_in6),
							host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
			if (s != 0) {
				printf("getnameinfo() failed: %s\n", gai_strerror(s));
				exit(EXIT_FAILURE);
			}
			printf("\t<%s>\n", host);
			if(ifa->ifa_name[0] != 'l' || ifa->ifa_name[1] == 'o')
			{
				res = malloc(sizeof(char)*NI_MAXHOST);
				strcpy(res, host);
				this_site.broadcastAdd = (((((struct sockaddr_in*)(ifa->ifa_netmask))->sin_addr.s_addr) ^ (unsigned int)(pow(2,32)-1)) | inet_addr(res));
			}
		}
	}
	
    printf("Adresse choisie : %s\n", res);
	printf("Adresse de broadcast : %s\n", inet_ntoa(*(struct in_addr *)&this_site.broadcastAdd));
	freeifaddrs(ifaddr);
	return res;
}


int gstArgs(int argc, char* argv[])
{
	//int i = 1;
	
	/*if(argc == 2 && strcmp(argv[1], "--help")==0)
	{
		printf("Arguments : ");
		printf("[masque_sous_reseau]\n");
		return -1;
	}*/

	/*if(argc < 2)
	{
		printf("Ce programme necessite des arguments. Essayez '--help' pour plus de precisions.\n\n");
		return -1;
	}
	
	while(i < argc)
	{
		if(inet_aton(argv[i], &addMask) != 0)
		{
			//printf("Mask : %s\n", inet_ntoa(addMask));
			i++;
		}
		else
		{
			printf("Veuillez entrer des arguments coherents.\n");
			exit(EXIT_FAILURE);
		}
	}*/
	
	this_site.sdSend = socket(AF_INET, SOCK_DGRAM, 0);
	if(this_site.sdSend < 0)
	{
		perror("Erreur d'ouverture de socket ");
		return -1;
	}
	
	this_site.sdRecv = socket(AF_INET, SOCK_DGRAM, 0);
	if(this_site.sdRecv < 0)
	{
		perror("Erreur d'ouverture de socket ");
		return -1;
	}
	
	int enabled = 1;
	setsockopt(this_site.sdSend, SOL_SOCKET, SO_BROADCAST, &enabled, sizeof(enabled));
	setsockopt(this_site.sdRecv, SOL_SOCKET, SO_BROADCAST, &enabled, sizeof(enabled));
	
	char* addr = getIPaddress();
	struct sockaddr_in myNetParams;
	bzero(&myNetParams,sizeof(myNetParams));
	myNetParams.sin_family = AF_INET;
	myNetParams.sin_addr.s_addr = inet_addr(addr);
	myNetParams.sin_port = htons(PORT_SEND);
	//memset(myNetParams->sin_zero,0,8);
	free(addr);
	
	if(bind(this_site.sdSend, (struct sockaddr*)&myNetParams, (socklen_t)sizeof(myNetParams)) == -1)
	{
		perror("Erreur de lien a la boite reseau ");
		return -1;
	}
	
	myNetParams.sin_family = AF_INET;
	myNetParams.sin_addr.s_addr = INADDR_ANY;
	myNetParams.sin_port = htons(PORT_RECV);
	
	if(bind(this_site.sdRecv, (struct sockaddr*)&myNetParams, (socklen_t)sizeof(myNetParams)) == -1)
	{
		perror("Erreur de lien a la boite reseau ");
		return -1;
	}
	
	this_site.neighbours = NULL;
	this_site.neighboursTmp = NULL;
	this_site.nbNeighbours = 0;
	this_site.running = 1;
	printf("Lancement du site…\n");
	
	return 0;
}

char* itoa(long n)
{
	int i = 1, nbChiffres;
	long trans = n;
	while(trans > 10)
	{
		trans /= 10;
		i++;
	}
	nbChiffres = i;
	char* chaine = malloc((nbChiffres+1) * sizeof(char));
	trans = n;
	
	for(i = 0 ; i < nbChiffres ; i++)
	{
		chaine[nbChiffres-1-i] = 48 + trans % 10;
		trans /= 10;
	}
	chaine[nbChiffres] = 0;
	
	return chaine;
}

int backupSocketNeighbours()
{
	int j;
	this_site.neighboursTmp = malloc(sizeof(struct sockaddr_in)*this_site.nbNeighbours);
	if(this_site.neighboursTmp == NULL)
	{
		perror("Erreur d'allocation ");
		return -1;
	}
	for(j = 0 ; j < this_site.nbNeighbours ; j++)
	{
		this_site.neighboursTmp[j] = this_site.neighbours[j];
	}
	
	return 0;
}

int recoverSocketNeighbours(struct sockaddr_in paramsNewNeighbour)
{
	int j;
	this_site.neighbours = realloc(this_site.neighbours, sizeof(struct sockaddr_in)*this_site.nbNeighbours);
	if(this_site.neighbours == NULL)
	{
		perror("Erreur de reallocation ");
		return -1;
	}
	
	bzero(&((this_site.neighbours)[this_site.nbNeighbours-1]),sizeof((this_site.neighbours)[this_site.nbNeighbours-1]));
	((this_site.neighbours)[this_site.nbNeighbours-1]).sin_family = paramsNewNeighbour.sin_family;
	((this_site.neighbours)[this_site.nbNeighbours-1]).sin_port = paramsNewNeighbour.sin_port;
	((this_site.neighbours)[this_site.nbNeighbours-1]).sin_addr.s_addr = paramsNewNeighbour.sin_addr.s_addr;
	
	for(j = 0 ; j < this_site.nbNeighbours-1 ; j++)
	{
		(this_site.neighbours)[j] = this_site.neighboursTmp[j];
	}
	free(this_site.neighboursTmp);
	
	return 0;
}

int broadcast(message* m)
{
	struct sockaddr_in netParamsNeighbour;
	bzero(&netParamsNeighbour,sizeof(netParamsNeighbour));
	netParamsNeighbour.sin_family = AF_INET;
	netParamsNeighbour.sin_port = htons(PORT_RECV);
	netParamsNeighbour.sin_addr.s_addr = this_site.broadcastAdd;
	if (sendto(this_site.sdSend, m, sizeof(*m)+1, 0, (struct sockaddr *)&netParamsNeighbour,sizeof(netParamsNeighbour)) == -1)
	{
		perror("sendto broadcast ");
		return -1;
	}
	return 0;
}

int sendMessage(int siteID, message* m)
{
	struct sockaddr_in netParamsNeighbour;
	bzero(&netParamsNeighbour,sizeof(netParamsNeighbour));
	netParamsNeighbour = this_site.neighbours[siteID];
	if (sendto(this_site.sdSend, m, sizeof(*m)+1, 0, (struct sockaddr *)&netParamsNeighbour,sizeof(netParamsNeighbour)) == -1)
	{
		perror("sendto message ");
		return -1;
	}
	return 0;
}

int sendMessageWithAdd(char* add, message* m)
{
	struct sockaddr_in netParamsNeighbour;
	bzero(&netParamsNeighbour,sizeof(netParamsNeighbour));
	netParamsNeighbour.sin_family = AF_INET;
	netParamsNeighbour.sin_port = htons(PORT_RECV);
	netParamsNeighbour.sin_addr.s_addr = inet_addr(add);
	if (sendto(this_site.sdSend, m, sizeof(*m)+1, 0, (struct sockaddr *)&netParamsNeighbour,sizeof(netParamsNeighbour)) == -1)
	{
		perror("sendto message ");
		return -1;
	}
	return 0;
}

void standardInput()
{
	int nbLus;
	message msg = {MESSAGE, NULL};
	msg.content = malloc(sizeof(char)*1024);
	char buf[50];
	nbLus = read(STDIN_FILENO, buf, 49);
	if(nbLus < 1)
		perror("Erreur de reception sur l'entree standard");
	else
		buf[nbLus-1] = 0;
	
	
	if(strstr(buf, "BROADCAST") != NULL)
	{
		if(strlen(buf) == strlen("BROADCAST"))
		{
			printf("Entrez le message : \n");
			nbLus = read(STDIN_FILENO, buf, 49);
			if(nbLus < 1)
				perror("Erreur de reception sur l'entree standard");
			else
				buf[nbLus-1] = 0;
			strcpy(msg.content, buf);
		}
		else
			strcpy(msg.content, buf+strlen("BROADCAST")+1);
		broadcast(&msg);
	}
	else if(strstr(buf, "MESSAGE") != NULL)
	{
		printf("TO : \n");
		nbLus = read(STDIN_FILENO, buf, 49);
		if(nbLus < 1)
			perror("Erreur de reception sur l'entree standard");
		else
			buf[nbLus-1] = 0;
		
		char add[256];
		strcpy(add, buf);
		
		printf("CORPS DU TEXTE : \n");
		nbLus = read(STDIN_FILENO, buf, 49);
		if(nbLus < 1)
			perror("Erreur de reception sur l'entree standard");
		else
			buf[nbLus-1] = 0;
		
		strcpy(msg.content, buf);
		sendMessageWithAdd(add, &msg);
	}
	else if(strcmp(buf, "NEIGHBOURS") == 0)
	{
		printNeighbours();
	}
	else
	{
		printf("Entree standard recue non traitee.\n");
	}
	free(msg.content);
}

int hostsUpdate(struct sockaddr_in netParamsNeighbour)
{
	int known = 0, i;
	
	for(i = 0 ; i < this_site.nbNeighbours ; i++)
	{
		if(ntohs(netParamsNeighbour.sin_addr.s_addr) == ntohs(this_site.neighbours[i].sin_addr.s_addr))
		{
			known = 1;
		}
	}
	if(known == 0)
	{
		printf("Hote inconnu. ");
		// backup neighbours sockets
		if(backupSocketNeighbours() == -1)
		{
			return -1;
		}
		
		this_site.nbNeighbours++;
		
		// recover neighbours sockets
		if(recoverSocketNeighbours(netParamsNeighbour) == -1)
		{
			return -1;
		}
	}
	else
	{
		printf("Hote connu. ");
	}
	
	return 0;
}

void requestTreatment()
{
	
}

void printNeighbours()
{
	int i;
	printf("Neighbours : ");
	for(i = 0 ; i < this_site.nbNeighbours ; i++)
	{
		printf("%s ", inet_ntoa(this_site.neighbours[i].sin_addr));
	}
	printf("\n");
}


