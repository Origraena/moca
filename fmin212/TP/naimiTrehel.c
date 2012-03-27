//
//  naimiTrehel.c
//  
//
//  Created by Chloe Desdouits on 20/03/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//


#include "naimiTrehel.h"

int init_structures()
{
	int i;
	last = -1;
	next = -1;
	state = IDLE;
	tokenPresent = 0;
	
	/* HELLO message broadcasting */
	msg_type t = HELLO;
	broadcast(t, "");
	
	printf("\nAttente d'autres sites...\n");
	waitForHellorep(WAITING_PERIOD);
	
	/* HELLO message broadcasting */
	//broadcast(t, "");
	
	
	if(this_site.nbNeighbours == 1)
	{
		tokenPresent = 1;
	}
	else if(last == -1)
	{
		unsigned long int ip_max = 0;
		for(i = 0 ; i < this_site.nbNeighbours ; i++)
		{
			if((unsigned long int)(this_site.neighbours[i].sin_addr.s_addr) > ip_max)
			{
				ip_max = this_site.neighbours[i].sin_addr.s_addr;
				last = i;
			}
		}
		tokenPresent = (last == 0 ? 1 : 0);
		last = -1;
	}
	
	
	return 0;
}

int critSectionRequest()
{
	state = WAITING;
	msg_type t = REQUEST;
	char* ipAddr;
	
	if(tokenPresent == 1)
	{
		takeCriticalSection();
	}
	else if(last != -1)
	{
		// send a request to last
		itoa(this_site.neighbours[0].sin_addr.s_addr, &ipAddr);
		//printf("addr %ul ; apres itoa %s\n", this_site.neighbours[0].sin_addr.s_addr, ipAddr);
		if(sendMessage(last, t, ipAddr) == -1)
		{
			free(ipAddr);
			return -1;
		}
		free(ipAddr);
	}
	else
	{
		itoa(this_site.neighbours[0].sin_addr.s_addr, &ipAddr);
		if(broadcast(t, ipAddr) == -1)
		{
			free(ipAddr);
			return -1;
		}
		free(ipAddr);
	}
	last = -1;
	
	return 0;
}

int handleMessage(int type, char* message)
{
	switch (type)
	{
		case REQUEST:
			return handleRequest(message);
			break;
			
		case TOKEN:
			return handleToken(message);
			break;
			
		case HELLO:
			return handleHello(message);
			break;
			
		case HELLOREP:
			return handleHelloRep(message, NULL);
			break;
			
		default:
			fprintf(stderr, "Type de message receptionne inconnu...\n");
			return -1;
			break;
	}
}

int handleRequest(char* ip)
{
	msg_type t;
	
	if(getNeighbour(atoi(ip)) == 0)
		return 0;
	
	if(last == -1)
	{
		if(state == WAITING || state == WORKING)
		{
			next = getNeighbour(atoi(ip));
		}
		else if(tokenPresent == 1)
		{
			//printf("request answer atoll(ip) %lu\n", (unsigned long int)atoll(ip));
			t = TOKEN;
			if(sendMessage(getNeighbour((unsigned long int)atoll(ip)), t, "") == -1)
			{
				return -1;
			}
			tokenPresent = 0;
		}
	}
	else
	{
		t = REQUEST;
		if(sendMessage(last, t, ip) == -1)
		{
			return -1;
		}
	}
	last = getNeighbour(atoi(ip));
	
	return 0;
}

int handleToken(char* message)
{
	tokenPresent = 1;
	if(state == WAITING)
	{
		takeCriticalSection();
	}
	
	return 0;
}

int takeCriticalSection()
{
	printf("Prise de la section critique\n");
	state = WORKING;
	
	/* exec CS */
	pthread_t thread_id;
	if(pthread_create(&thread_id, NULL, (void*)(liberation), (void*)20) != 0)
	{
		fprintf(stderr, "Thread creation failure.\n");
	}
	
	return 0;
}

void liberation(void* arg)
{
	pthread_detach(pthread_self());
	
	
	sleep((long int)arg);
	
	
	state = IDLE;
	if(next != -1)
	{
		msg_type t = TOKEN;
		if(sendMessage(next, t, "") == -1)
		{
			
		}
		next = -1;
		tokenPresent = 0;
	}
	printf("Section critique relachee\n");
}

int handleHello(char* message)
{
	msg_type t = HELLOREP;
	char* ipLastStr;
	if(last == -1)
	{
		ipLastStr = malloc(4*sizeof(char));
		ipLastStr[0] = '-';
		ipLastStr[1] = '1';
		ipLastStr[2] = 48+tokenPresent;
		ipLastStr[3] = 0;
	}
	else
		getIPstrFromNb(last, &ipLastStr);
	int res = broadcast(t, ipLastStr);
	free(ipLastStr);
	return res;
}

int handleHelloRep(char* message, struct sockaddr_in* netParamsNeighbour)
{
	long long int ipLastJ = atoi(message);
	int lastJ = -1, i;
	
	if(last == -1)
	{
		if(ipLastJ > 0)
		{
			for(i = 0 ; i < this_site.nbNeighbours ; i++)
			{
				if((unsigned long int)(this_site.neighbours[i].sin_addr.s_addr) == (unsigned long int)(ipLastJ))
				{
					lastJ = i;
				}
			}
			last = lastJ;
		}
		else
		{
			if(ipLastJ == -10)
			{
				
			}
			else if(ipLastJ == -11)
			{
				if(netParamsNeighbour != NULL)
				{
					printf("Hellorep recu et token present chez l'autre\n");
					for(i = 0 ; i < this_site.nbNeighbours ; i++)
					{
						if((unsigned long int)(this_site.neighbours[i].sin_addr.s_addr) == (unsigned long int)(netParamsNeighbour->sin_addr.s_addr))
						{
							lastJ = i;
						}
					}
					last = lastJ;
				}
			}
		}
	}
	
	return 0;
}

int waitForHellorep(int waitingPeriod)
{
	struct sockaddr_in netParamsNeighbour;
	time_t timeStart, timeCur;
	timeStart = time(&timeStart);
	timeCur = time(&timeCur);
	
	char* msg;
	msg_type t;
	
	int flags = fcntl(this_site.sdRecv, F_GETFL);
	int flags2 = flags | O_NONBLOCK;
	fcntl(this_site.sdRecv, F_SETFL, flags2);
	
	//while((this_site.nbNeighbours < 2) || (timeCur - timeStart < waitingPeriod))
	while(timeCur - timeStart < waitingPeriod)
	{
		timeCur = time(&timeCur);
		msg = NULL;
		
		if(recvMessage(&t, &msg, &netParamsNeighbour) == -1)
		{
			if(msg != NULL)
				free(msg);
			continue;
		}
		
		
		switch (t)
		{
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
		
		if(msg != NULL)
			free(msg);
		
		printf("Attente d'autres sites...\n");
	}
	
	fcntl(this_site.sdRecv, F_SETFL, flags);
	
	printf("Phase d'initialisation du reseau terminee.\n%ld sites trouves.\n\n", (long int)this_site.nbNeighbours);
	
	return 0;
}


