//
//  naimiTrehel.c
//  
//
//  Created by Chloe Desdouits on 20/03/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "naimiTrehel.h"

void init_structures()
{
	next = -1;
	last = -1;
	tokenPresent = 0;
	state = IDLE;
}

int critSectionRequest()
{
	msg_type t = REQUEST;
	char* ipAddr;
	
	if(last != -1)
	{
		// send a request to last
		itoa(this_site.neighbours[0].sin_addr.s_addr, &ipAddr);
		if(sendMessage(last, t, ipAddr) == -1)
		{
			return -1;
		}
		free(ipAddr);
		last = 0;
		state = WAITING;
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
	switch (type)
	{
		case REQUEST:
			return handleRequest(message);
			break;
			
		case TOKEN:
			return handleToken(message);
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
		last = getNeighbour(atoi(ip));
		if(last == -1)
		{
			fprintf(stderr, "Request depuis un site inconnu...\n");
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
	if(pthread_create(&thread_id, NULL, (void*)(execCS), (void*)1) != 0)
	{
		fprintf(stderr, "Thread creation failure.\n");
	}
	
	state = IDLE;
	if(next != -1)
	{
		msg_type t = TOKEN;
		if(sendMessage(next, t, "") == -1)
		{
			return -1;
		}
		tokenPresent = 0;
	}
	
	return 0;
}

void execCS(void* arg)
{
	sleep((long int)arg);
};


