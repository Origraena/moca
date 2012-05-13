#include "naimiTrehel.h"

#ifndef TMESG
#define TMESG 2
#endif

//{{{ init_structures
int init_structures() {
	int i;
	last = -1;
	next = -1;
	state = IDLE;
	tokenPresent = 0;


	/* HELLO message broadcasting */
    msg_t broad;
    memset(&broad, 0, SIZE);
    type(broad) = HELLO;
    broadcast(broad);

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

	return 0;
}
// }}}

// {{{ critSectionRequest
// Ask for critical section
int critSectionRequest() {
	msg_t msg;
	type(msg) = REQUEST;
	char *ip_tmp = inet_ntoa(this_site.neighbours[0].sin_addr);
	strncpy (ask(msg), ip_tmp, IPLONG);

	// If already in SC just return -1
	if (state != IDLE) {
		fprintf (stderr,"Already in critical section.\n");
		return -1;
	}
	state = WAITING;

	// Owns Token ?
	if(tokenPresent) 
		return takeCriticalSection();
	// Sends request to last
	else if(last != -1) {
		// Get ip of last
		char *tmpter = getIPstrFromNb (last);
		fprintf (stdout, "Last is %d, sending it a request.\n", last);
		strncpy(ip(msg), tmpter, IPLONG);
		free (tmpter);

		strncpy (ask(msg), inet_ntoa(this_site.neighbours[0].sin_addr), IPLONG);

		if(sendMessageWithAdd(msg) == -1){
			fprintf (stderr, "======> Sending request failure... <======\n");
			return -1;
		}
	}
	last = -1;

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
		default:
			fprintf(stderr, "======> Uknown message type <======\n");
			return -1;
	}
}
// }}}

// {{{ handleRequest
int handleRequest(msg_t msg) {
	strncpy(ip(msg), ask(msg), IPLONG * sizeof(char));
	unsigned long int ipt = (unsigned long int)inet_addr(ask(msg));

	// TODO: Add unknown Neighbour to the list of neighbour and handle REEQUEST
	if(getNeighbour(ipt)==-1) {
		fprintf (stderr, "======> Received a REQUEST from an unknown neighbour. <======\n");
		return 0;
	}

	if(last == -1) {
		if(state != IDLE) {
			if (next == -1) {
				next = getNeighbour(ipt);
				last = next;
				fprintf (stdout, "Got a request, but expecting the TOKEN.\n");
				type(msg) = COMMIT;
				if (sendMessageWithAdd(msg) == -1)
					return -1;
			}
		}
		else if(tokenPresent == 1) {
			fprintf (stdout, "Sending TOKEN.\n");
			type(msg) = TOKEN;
			if(sendMessageWithAdd(msg) == -1){
				fprintf (stderr, "======> Error while sending message <======\n");
				return -1;
			}
			tokenPresent = 0;
		}
	}
	else {
		type(msg) = REQUEST;
		if(sendMessage(last, msg) == -1)
			return -1;
	}

	last = getNeighbour(ipt);
	fprintf (stdout, "New last after receiving request : %d. \n", last);

	return 0;
}
//}}}


//{{{ handleToken
int handleToken(msg_t message) {
	fprintf (stdout, "TOKEN Received. \n");
	tokenPresent = 1;
	if(state == WAITING)
		takeCriticalSection();

	return 0;
}
//}}}

//{{{ handleHello
int handleHello(msg_t mes) {
	type(mes) = HELLOREP;
	tok(mes) = tokenPresent;

	fprintf (stdout, "HELLO received, sending HELLOREP\n");

	strncpy(ip(mes), ips(mes), IPLONG);

	return sendMessageWithAdd(mes);
}
//}}}

//{{{ handleHelloRep
int handleHelloRep(msg_t message, struct sockaddr_in* netParamsNeighbour) {
	unsigned long int ipLastJ = (unsigned long int) inet_addr(ips(message));
	int i;

	if (tok(message)) {
		for (i=0; i<this_site.nbNeighbours; i++)
			if ((unsigned long int)this_site.neighbours[i].sin_addr.s_addr == ipLastJ){
				last = i;
				fprintf(stdout, "Get HELLOREP. TOKEN on site %d.\n", last);
				if (i) {
					tokenPresent = 0;
					fprintf (stdout, "Throw up TOKEN\n");
				}
			}
	}
	else 
		fprintf(stdout, "HELLOREP, but no TOKEN.\n");

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

	fprintf (stdout, "Expecting HELLOREP for network discover.\n");

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

			case RESOURCE:
				handleResource(msg);
				break;

			default:
				fprintf(stderr, "======> Unknown message type <======\n");
				break;
		}
	}

	fcntl(this_site.sdRecv, F_SETFL, flags);

	printf("End of network discovery : %ld sites found.\n", (long int)this_site.nbNeighbours);

	return 0;
}
//}}}
//}}}


//{{{ Critical Section + Threads
//{{{ takeCriticalSection
int takeCriticalSection() {
	fprintf(stdout, "Entering CS\n");
	state = WORKING;

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

	state = IDLE;
	if(next != -1) {
		fprintf (stdout, "Sending TOKEN.\n");
		msg_t mes;
		type(mes) = TOKEN;
		if(sendMessage(next, mes) == -1) 
			fprintf(stderr, "======> Error while sending message <====== \n");
		next = -1;
		tokenPresent = 0;
	}
	else
		last = -1;
}
//}}}

//}}}

int handleResource(msg_t msg) {
	strcpy(this_site.resource,msg._sender);
	return 1;
}

int handleSolution(msg_t msg) {
	int w1,w2;
	w1 = msg._nb_access;
	w2 = msg._has_next;
	if (this_problem.w1 < w1) {
		printf("[%s] New solution found : %i / %i !\n",msg._sender,w1,w2);
		this_problem.w1 = w1;
		this_problem.w2 = w2;
	}
	else
		if (_verbose) printf("[%s] Solution received but unused : %i / %i...\n",msg._sender,w1,w2);
	return 1;
}



