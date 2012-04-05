#include "naimiTrehel.h"

#ifndef TMESG
#define TMESG 2
#endif

//{{{ Header
int handleCommit(msg_t msg);
int handleAreYouAlive(msg_t msg);
int handleSearchPrev (msg_t msg);
int handleSearchQueue(msg_t msg);
int handleConnection (msg_t msg);
int handleAckSearchPrev (msg_t msg);
int handleAckSearchQueue (msg_t msg);
int handleIAmAlive (msg_t msg);

void checkNeighbour (void *arg);
//}}}

//{{{ site_failure
void site_failure(int sig) {
	printf("Réception du signal, site mort.\n");
	int i=0;
	int sortie = 0;
	msg_t msg;
	time_t timeStart, timeCur;

	int flags = fcntl(this_site.sdRecv, F_GETFL);
	int flags2 = flags | O_NONBLOCK;
	fcntl(this_site.sdRecv, F_SETFL, flags2);

	for (i=1; i<TOLERANCE+1; i++) {
		type(msg) = ARE_YOU_ALIVE;
		char *tmp = inet_ntoa(predec[i].sin_addr);
		strncpy (ip(msg), tmp, IPLONG *sizeof(char));
		unsigned long int ipa = (unsigned long int) inet_addr(ip(msg));
		sendMessageWithAdd(msg);

		pthread_mutex_lock(&mut_check);
		if (check)
			check++;
		pthread_mutex_unlock(&mut_check);

		timeStart = time(&timeStart);
		timeCur = time(&timeCur);

		while(timeCur - timeStart < (2*TMESG)) {
			timeCur = time(&timeCur);
			memset (&msg, 0, SIZE);

			if(recvMessage(&msg, NULL) == -1) 
				continue;

			if (type(msg) == I_AM_ALIVE) {
				handleIAmAlive(msg);
				type(msg) = CONNECTION;
				strncpy(ip(msg), ips(msg), IPLONG * sizeof(char));
				sendMessageWithAdd(msg);
				last = getNeighbour(ipa);
				critSectionRequest();
				sortie++;
				break;
			}
			else if (type(msg) == REQUEST) {}
			else
				handleMessage(msg);
		}
	}

	if (!sortie) {
		msg_t min;

		type(msg) = SEARCH_PREV;
		pos(msg) = position;
		broadcast(msg);

		timeStart = time(&timeStart);
		timeCur = time(&timeCur);

		while(timeCur - timeStart < (2*TMESG)) {
			timeCur = time(&timeCur);
			memset (&msg, 0, SIZE);

			if(recvMessage(&msg, NULL) == -1) 
				continue;

			pos(min) = -1;
			if (type(msg) == ACK_SEARCH_PREV) 
				min = pos(min) == -1 ? msg : (pos(min) > pos(msg) ? msg : min);
			else if (type(msg) == REQUEST) {}
			else
				handleMessage(msg);
		}

		if (pos(min) == -1) {
			tokenPresent = 1;
			critSectionRequest();
			sortie ++;
		}

		if (!sortie) {
			type(msg) = CONNECTION;
			strncpy(ip(msg), ips(min), IPLONG * sizeof(char));
			last = getNeighbour((unsigned long int)inet_addr(ips(min)));
			takeCriticalSection();
		}
	}
}
//}}}

//{{{ init_structures
int init_structures() {
	int i;
	last = -1;
	next = -1;
	state = IDLE;
	tokenPresent = 0;

	position = -1;
	pthread_mutex_init(&mut_check, NULL);
	check = 0;
	acces = 0;
	for (i=0; i<TOLERANCE+1; memset(&predec[i++], 0, sizeof(struct sockaddr_in)));

	/* HELLO message broadcasting */
	msg_t broad;
	type(broad) = HELLO;
	broadcast(broad);

	printf("\nAttente d'autres sites...\n");
	waitForHellorep(WAITING_PERIOD);

	/* HELLO message broadcasting */
	//broadcast(t, "");


	if(this_site.nbNeighbours == 1)
		tokenPresent = 1;
	else if(last == -1) {
		unsigned long int ip_max = 0;
		for(i = 0 ; i < this_site.nbNeighbours ; i++)
			if((unsigned long int)(this_site.neighbours[i].sin_addr.s_addr) > ip_max) {
				ip_max = this_site.neighbours[i].sin_addr.s_addr;
				last = i;
			}
		tokenPresent = (!last ? 1 : 0);
		if (tokenPresent)
			last = -1;
	}

	struct sigaction action;
	memset(&action, 0, sizeof(action));
	action.sa_handler = site_failure;
	if (sigaction(SIGUSR1,&action,NULL)){
		perror ("sigaction ");
		return -1;
	}

	return 0;
}
// }}}

// {{{ critSectionRequest
int critSectionRequest() {
	msg_t msg;
	type(msg) = REQUEST;
	state = WAITING;

	if(tokenPresent == 1) {
		takeCriticalSection();
		return 0;
	}
	else if(last != -1) {
		char *tmpter = getIPstrFromNb (last);
		printf ("Envoi message à last : %d, adresse : %s.\n", last, tmpter);
		free (tmpter);
		if(sendMessage(last, msg) == -1)
			return -1;

		time_t timeStart, timeCur;
		int flags = fcntl(this_site.sdRecv, F_GETFL);
		int flags2 = flags | O_NONBLOCK;
		fcntl(this_site.sdRecv, F_SETFL, flags2);

		timeStart = time(&timeStart);
		timeCur = time(&timeCur);

		while(timeCur - timeStart < (2*TMESG)) {
			timeCur = time(&timeCur);

			if(recvMessage(&msg, NULL) == -1) 
				continue;

			if (type(msg) == COMMIT){
				printf("Réception Commit\n");
				return handleCommit(msg);
			}
			else {
				if (type(msg) == REQUEST) {}
				else if (type(msg) == TOKEN) {
					handleToken(msg);
					return 0;
				}
				else
					handleMessage(msg);
			}
		}

		type(msg) = SEARCH_QUEUE;
		nb_acc(msg) = acces;
		broadcast(msg);

		msg_t max;
		pos(max) = -1;

		timeStart = time(&timeStart);
		timeCur = time(&timeCur);

		while(timeCur - timeStart < 2*TMESG) {
			timeCur = time(&timeCur);
			memset (&msg, 0, SIZE);

			if(recvMessage(&msg, NULL) == -1) 
				continue;

			switch(type(msg)) {
				case ACK_SEARCH_QUEUE:
					if (pos(msg) > pos(max))
						max = msg;
					break;
				case SEARCH_QUEUE:
					if (nb_acc(msg) > acces) 
						continue;
					if (nb_acc(msg) == acces) {
						char *ip_pers = inet_ntoa(this_site.neighbours[0].sin_addr);
						if (strcmp(ip_pers, ips(msg)) <= 0)
							continue;
					}
					unsigned long int ipa = (unsigned long int) inet_addr(ips(msg));
					last = getNeighbour(ipa);
					return critSectionRequest();
				case REQUEST:
					break;
				default:
					handleMessage(msg);
					break;
			}
		}

		if (pos(max) < 0) {
			tokenPresent = 1;
			takeCriticalSection();
		}
		else {
			unsigned long int ipa = (unsigned long int) inet_addr(ip(msg));
			last = getNeighbour(ipa);
			strncpy(ip(msg), ips(max),IPLONG *sizeof(char));
			if (next(msg)) {
				type(msg) = CONNECTION;
				if (sendMessageWithAdd(msg) == -1)
					return -1;
			}
			return critSectionRequest();
		}
	}
	else{
		fprintf (stderr, "Rien ne se passe comme prévu...\n");
	}
	return 0;
}
// }}}

// {{{ Message Management
// {{{ handleMessage
int handleMessage(msg_t msg) {
	switch (type(msg)) {
		case REQUEST:
			return handleRequest(msg);
		case TOKEN:
			return handleToken(msg);
		case HELLO:
			return handleHello(msg);
		case HELLOREP:
			return handleHelloRep(msg, NULL);
		case ARE_YOU_ALIVE:
			return handleAreYouAlive(msg);
		case SEARCH_PREV:
			return handleSearchPrev(msg);
		case SEARCH_QUEUE:
			return handleSearchQueue(msg);
		case CONNECTION:
			return handleConnection(msg);
		case I_AM_ALIVE:
			return handleIAmAlive(msg);
		default:
			fprintf(stderr, "Type de message receptionne inconnu...\n");
			return -1;
	}
}
// }}}

// {{{ handleRequest
int handleRequest(msg_t msg) {
	printf ("Adresses %s %s", ips(msg), ip(msg));
	strncpy(ip(msg), ips(msg), IPLONG * sizeof(char));
	printf ("Adresses %s %s", ips(msg), ip(msg));
	unsigned long int ipa = (unsigned long int)inet_addr(ips(msg));

	if(getNeighbour(ipa)==-1) {
		printf ("C'est la loose !!!\n");
		return 0;
	}

	if(last == -1) {
		if(state == WAITING || state == WORKING) {
			next = getNeighbour(ipa);
			printf("Demande reçue du voisin : %d\n", next);
			type(msg) = COMMIT;
			pos(msg) = position + 1;
			int j;
			for (j=0; j<TOLERANCE; pred(msg)[j] = predec[j], j++);
			if (sendMessageWithAdd(msg) == -1)
				return -1;
		}
		else if(tokenPresent == 1) {
			printf("request answer inet_addr(ip) %lu\n", (unsigned long int)ipa);
			type(msg) = TOKEN;
			if(sendMessageWithAdd(msg) == -1){
				printf ("Raté...\n");
				return -1;
			}
			tokenPresent = 0;
			position = -1;
		}
	}
	else {
		type(msg) = REQUEST;
		if(sendMessage(last, msg) == -1)
			return -1;
	}
	last = getNeighbour(ipa);

	return 0;
}
//}}}

//{{{ handleCommit
int handleCommit (msg_t msg) {
	unsigned long int ipa = (unsigned long int)inet_addr(ips(msg));
	pthread_t thread_id;

	if(!getNeighbour(ipa))
		return 0;

	position = pos(msg);
	int i;
	for (i=0; i<TOLERANCE; predec[i] = pred(msg)[i+1], i++);

	inet_aton(ips(msg),&(predec[0].sin_addr));

	if(pthread_create(&thread_id, NULL, (void*)(checkNeighbour), (void*)predec) != 0)
		fprintf(stderr, "Thread creation failure.\n");

	return 0;
}
//}}}

//{{{ handleToken
int handleToken(msg_t message) {
	tokenPresent = 1;
	if(state == WAITING)
		takeCriticalSection();
	else if (next != -1) {
		type(message) = TOKEN;
		char *tmp = getIPstrFromNb(next);
		strncpy (ip(message), tmp, IPLONG);
		sendMessageWithAdd(message);
	}

	return 0;
}
//}}}

//{{{ handleHello
int handleHello(msg_t mes) {
	type(mes) = HELLOREP;
	char* ipLastStr;
	if(last == -1) {
		ipLastStr = malloc(4*sizeof(char));
		ipLastStr[0] = '-';
		ipLastStr[1] = '1';
		ipLastStr[2] = 48+tokenPresent;
		ipLastStr[3] = 0;
	}
	else
		ipLastStr = getIPstrFromNb(last);

	printf ("Voilà ce que je balance : %s\n", ipLastStr);
	memcpy (&(ip(mes)), ipLastStr, (strlen(ipLastStr) + 1) * sizeof(char));

	int res = broadcast(mes);
	free(ipLastStr);
	return res;
}
//}}}

//{{{ handleHelloRep
int handleHelloRep(msg_t message, struct sockaddr_in* netParamsNeighbour) {
	long long int ipLastJ = atoi(ip(message));
	int lastJ = -1, i;

	printf ("Voilà ce que je mange : %s --> %lld\n", ip(message), ipLastJ);

	if(last == -1) {
		if(ipLastJ > 0) {
			for(i = 0 ; i < this_site.nbNeighbours ; i++)
				if((unsigned long int)(this_site.neighbours[i].sin_addr.s_addr) == (unsigned long int)(netParamsNeighbour[i].sin_addr.s_addr))
					lastJ = i;
			last = lastJ;
			printf ("Tout va bien dans le meilleur des mondes :\n");
			for (i=0; i< this_site.nbNeighbours; printf("%s - ",inet_ntoa(this_site.neighbours[i++].sin_addr)));
			printf ("\n");
		}
		else {
			//			if(ipLastJ == -10)
			if(ipLastJ == -11) {
				if(netParamsNeighbour != NULL) {
					printf("Hellorep recu et token present chez l'autre\n");
					for(i = 0 ; i < this_site.nbNeighbours ; i++)
						if((unsigned long int)(this_site.neighbours[i].sin_addr.s_addr) == (unsigned long int)(netParamsNeighbour->sin_addr.s_addr))
							lastJ = i;
					last = lastJ;
					printf ("Tout va bien dans le meilleur des mondes num 2 :\n");
					for (i=0; i< this_site.nbNeighbours; printf("%s - ",inet_ntoa(this_site.neighbours[i++].sin_addr)));
					printf ("\n");
				}
			}
		}
	}

	return 0;
}
//}}}

//{{{ waitForHellorep
int waitForHellorep(int waitingPeriod) {
	struct sockaddr_in netParamsNeighbour;
	time_t timeStart, timeCur;
	timeStart = time(&timeStart);
	timeCur = time(&timeCur);

	msg_t msg;

	int flags = fcntl(this_site.sdRecv, F_GETFL);
	int flags2 = flags | O_NONBLOCK;
	fcntl(this_site.sdRecv, F_SETFL, flags2);

	//while((this_site.nbNeighbours < 2) || (timeCur - timeStart < waitingPeriod))
	while(timeCur - timeStart < waitingPeriod) {
		timeCur = time(&timeCur);
		memset (&msg, 0, SIZE);

		if(recvMessage(&msg, &netParamsNeighbour) == -1) 
			continue;

		switch (type(msg)) {
			case HELLO:
				handleHello(msg);
				break;

			case HELLOREP:
				handleHelloRep(msg, &netParamsNeighbour);
				break;

			default:
				fprintf(stderr, "Type de message receptionne inconnu...\n");
				break;
		}

		printf("Attente d'autres sites...\n");
	}

	fcntl(this_site.sdRecv, F_SETFL, flags);

	printf("Phase d'initialisation du reseau terminee.\n%ld sites trouves.\n\n", (long int)this_site.nbNeighbours);

	return 0;
}
//}}}

//{{{ handleAreYouAlive
int handleAreYouAlive(msg_t msg){
	type(msg) = I_AM_ALIVE;
	strncpy(ip(msg), ips(msg), IPLONG * sizeof(char));
	return (sendMessageWithAdd(msg));
}
//}}}

//{{{ handleSearchPrev
int handleSearchPrev (msg_t msg){
	if (position < pos(msg)) {
		type(msg) = ACK_SEARCH_PREV;
		pos(msg) = position;
		strncpy(ip(msg), ips(msg), IPLONG * sizeof(char));
		return (sendMessageWithAdd(msg));
	}
	return 0;
}
//}}}

//{{{ handleSearchQueue
int handleSearchQueue(msg_t msg){
	if (position >= 0) {
		type(msg) = ACK_SEARCH_QUEUE;
		pos(msg) = position;
		strncpy (ip(msg), ips(msg), IPLONG * sizeof(char));
		if (sendMessageWithAdd(msg) == -1)
			return -1;
	}
	return 0;
}
//}}}

//{{{ handleIAmAlive
int handleIAmAlive(msg_t m) {
	pthread_mutex_lock(&mut_check);
	if (check)
		check++;
	pthread_mutex_unlock(&mut_check);
	return 0;
}
//}}}

//{{{ handleAckSearchPrev
int handleAckSearchPrev(msg_t msg) {
	return 0;
}
//}}}

//{{{ handleAckSearchQueue
int handleAckSearchQueue(msg_t msg) {
	return 0;
}
//}}}

// {{{ handleConnection
int handleConnection (msg_t msg){
	next= -1;
	return 0;
}
//}}}
//}}}

//{{{ Critical Section + Threads
//{{{ takeCriticalSection
int takeCriticalSection() {
	printf("Prise de la section critique\n");
	state = WORKING;
	position = 0;
	int i;
	for (i=0; i<=TOLERANCE; memset(&(predec[i++]), 0, sizeof(struct sockaddr_in)));

	/* exec CS */
	pthread_t thread_id;
	if(pthread_create(&thread_id, NULL, (void*)(liberation), (void*)20) != 0)
		fprintf(stderr, "Thread creation failure.\n");

	return 0;
}
//}}}

//{{{ liberation
void liberation(void* arg) {
	pthread_detach(pthread_self());


	sleep((long int)arg);


	state = IDLE;
	if(next != -1) {
		printf ("Envoi du Token.\n");
		msg_t mes;
		type(mes) = TOKEN;
		if(sendMessage(next, mes) == -1) {
			printf("Envoi raté...");
		}
		next = -1;
		tokenPresent = 0;
	}
	printf("Section critique relachee : %d accès\n", ++acces);
}
//}}}

//{{{ checkNeighbour
void checkNeighbour (void *arg) {
	pthread_detach(pthread_self());

	printf("Je suis ici!\n");

	sigset_t block;
	sigemptyset (&block);
	sigaddset(&block, SIGUSR1);
	sigprocmask(SIG_BLOCK, &block, NULL);

	struct sockaddr_in *pre = (struct sockaddr_in *)arg;

	msg_t msg;
	type(msg) = ARE_YOU_ALIVE;
	char *tmpmes;
	tmpmes = inet_ntoa(pre->sin_addr);
	memcpy (ip(msg), tmpmes, IPLONG * sizeof(char));

	while (state == WAITING) {
		pthread_mutex_lock(&mut_check);
		if (check) {
			fprintf (stderr, "Site Failure Detected\n");
			pthread_mutex_unlock(&mut_check);
			kill(getpid(), SIGUSR1);
			exit(1);
		}
		check --;
		pthread_mutex_unlock(&mut_check);

		if (sendMessageWithAdd(msg) == -1)
			continue;

		sleep(2*TMESG);
	}
}
//}}}

//}}}


