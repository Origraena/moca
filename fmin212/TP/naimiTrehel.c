//
//  naimiTrehel.c
//  
//
//  Created by Chloe Desdouits on 20/03/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//


#include "naimiTrehel.h"

int init_structures() {
	int i;
	last = -1;
	next = -1;
	state = IDLE;
	tokenPresent = 0;
	
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
		last = -1;
	}
	
	return 0;
}

int critSectionRequest() {
	state = WAITING;
	msg_t msg;
	type(msg) = REQUEST;
	
	if(tokenPresent == 1)
		takeCriticalSection();
	else if(last != -1) {
		if(sendMessage(last, msg) == -1)
			return -1;
	}
	else 
		if(broadcast(msg) == -1)
			return -1;

	last = -1;
	
	return 0;
}

int handleMessage(msg_t msg) {
	switch (type(msg)) {
		case REQUEST:
			return handleRequest(msg);
			break;
			
		case TOKEN:
			return handleToken(msg);
			break;
			
		case HELLO:
			return handleHello(msg);
			break;
			
		case HELLOREP:
			return handleHelloRep(msg, NULL);
			break;
			
		default:
			fprintf(stderr, "Type de message receptionne inconnu...\n");
			return -1;
			break;
	}
}

int handleRequest(msg_t msg) {
	unsigned long int ipa = atoll(ips(msg));
	
	if(!getNeighbour(ipa))
		return 0;
	
	if(last == -1) {
		if(state == WAITING || state == WORKING)
			next = getNeighbour(ipa);
		else if(tokenPresent == 1) {
			//printf("request answer atoll(ip) %lu\n", (unsigned long int)atoll(ip));
			type(msg) = TOKEN;
			if(sendMessage(getNeighbour(ipa), msg) == -1)
				return -1;
			tokenPresent = 0;
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

int handleToken(msg_t message) {
	tokenPresent = 1;
	if(state == WAITING)
		takeCriticalSection();
	
	return 0;
}

int takeCriticalSection() {
	printf("Prise de la section critique\n");
	state = WORKING;
	
	/* exec CS */
	pthread_t thread_id;
	if(pthread_create(&thread_id, NULL, (void*)(liberation), (void*)20) != 0)
		fprintf(stderr, "Thread creation failure.\n");
	
	return 0;
}

void liberation(void* arg) {
	pthread_detach(pthread_self());
	
	
	sleep((long int)arg);
	
	
	state = IDLE;
	if(next != -1) {
		msg_t mes;
		type(mes) = TOKEN;
		if(sendMessage(next, mes) == -1) {}
		next = -1;
		tokenPresent = 0;
	}
	printf("Section critique relachee\n");
}

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
		getIPstrFromNb(last, &ipLastStr);

	memcpy (&(ip(mes)), ipLastStr, (strlen(ipLastStr) + 1) * sizeof(char));

	int res = broadcast(mes);
	free(ipLastStr);
	return res;
}

int handleHelloRep(msg_t message, struct sockaddr_in* netParamsNeighbour) {
	long long int ipLastJ = atoi(ip(message));
	int lastJ = -1, i;
	
	if(last == -1) {
		if(ipLastJ > 0) {
			for(i = 0 ; i < this_site.nbNeighbours ; i++)
				if((unsigned long int)(this_site.neighbours[i].sin_addr.s_addr) == (unsigned long int)(ipLastJ))
					lastJ = i;
			last = lastJ;
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
				}
			}
		}
	}
	
	return 0;
}

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


