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
	unsigned long int ip_max = 0;
	for(i = 0 ; i < this_site.nbNeighbours ; i++)
	{
		if((unsigned long int)(this_site.neighbours[i].sin_addr.s_addr) > ip_max)
		{
			ip_max = this_site.neighbours[i].sin_addr.s_addr;
			last = i;
		}
	}
	
	next = -1;
	state = IDLE;
	tokenPresent = (last == 0 ? 1 : 0);
	if(last == 0)
		last = -1;
	
	
	/* HELLO message broadcasting */
	msg_type t = HELLO;
	return broadcast(t, "") == -1;
}

int critSectionRequest()
{
	state = WAITING;
	msg_type t = REQUEST;
	char* ipAddr;
	
	if(tokenPresent == 1)
	{
		printf("Prise de la section critique\n");
		takeCriticalSection();
	}
	else if(last != -1)
	{
		// send a request to last
		itoa(this_site.neighbours[0].sin_addr.s_addr, &ipAddr);
		if(sendMessage(last, t, ipAddr) == -1)
		{
			return -1;
		}
		free(ipAddr);
		last = -1;
	}
	else
	{
		fprintf(stderr, "L'arbre logique n'est pas construit\n");
		return -1;
	}
	
	return 0;
}

int handleMessage(int type, char* message)
{
	msg_type t;
	
	switch (type)
	{
		case REQUEST:
			return handleRequest(message);
			break;
			
		case TOKEN:
			return handleToken(message);
			break;
			
		case HELLO:
			printf("HELLO\n");
			t = HELLOREP;
			char* lastStr;
			itoa(last, &lastStr);
			int res = broadcast(t, lastStr);
			free(lastStr);
			return res;
			break;
			
		case HELLOREP:
			return handleHelloRep(message);
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
	
	if(last != 0) /* this site is the root of the last tree */
	{
		t = REQUEST;
		if(sendMessage(last, t, ip) == -1)
		{
			return -1;
		}
	}
	else /* this site is NOT the root of the last tree */
	{
		if(tokenPresent == 1 && state == IDLE)
		{
			t = TOKEN;
			if(sendMessage(getNeighbour(atoi(ip)), t, "") == -1)
			{
				return -1;
			}
			tokenPresent = 0;
		}
		else
		{
			next = getNeighbour(atoi(ip));
		}
	}
	
	last = getNeighbour(atoi(ip));
	if(last == -1)
	{
		fprintf(stderr, "Request depuis un site inconnu...\n");
		return -1;
	}
	
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
	state = WORKING;
	
	/* exec CS */
	pthread_t thread_id;
	if(pthread_create(&thread_id, NULL, (void*)(execCS), (void*)5) != 0)
	{
		fprintf(stderr, "Thread creation failure.\n");
	}
	pthread_join(thread_id, NULL);
	
	state = IDLE;
	if(next != -1)
	{
		msg_type t = TOKEN;
		if(sendMessage(next, t, "") == -1)
		{
			return -1;
		}
		next = -1;
		tokenPresent = 0;
	}
	printf("Section critique relachee\n");
	
	return 0;
}

void execCS(void* arg)
{
	sleep((long int)arg);
};

int handleHelloRep(char* message)
{
	printf("HELLOREP\n");
	int lastJ = atoi(message);
	if(last == -1)
	{
		if(lastJ != -1)
		{
			last = lastJ;
		}
	}
	
	return 0;
}


