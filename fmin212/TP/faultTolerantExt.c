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

int isMe (char *ips);

void checkNeighbour (void *arg);
//}}}

//{{{ site_failure
void site_failure(int sig) {
	fprintf (stderr, "/* FAILURE OF A SITE SPOTTED */\n");
	int i=0;
	int sortie = 0;
	msg_t msg;
	time_t timeStart, timeCur;

	int flags = fcntl(this_site.sdRecv, F_GETFL);
	int flags2 = flags | O_NONBLOCK;
	fcntl(this_site.sdRecv, F_SETFL, flags2);

	// Sends ARE_YOU_ALIVE to other pred
	fprintf (stdout, "Checking predecessor validity\n");
	for (i=1; i<TOLERANCE+1; i++) {
		if (!strcmp ("0.0.0.0", inet_ntoa(predec[i].sin_addr))) {
			fprintf (stdout, "Bad ip address\n");
			continue;
		}
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

			// Receive I_AM_ALIVE --> Connection
			fprintf (stdout, "Received I_AM_ALIVE\n");
			if (type(msg) == I_AM_ALIVE && !isMe(ips(msg))) {
				handleIAmAlive(msg);
				type(msg) = CONNECTION;
				strncpy(ip(msg), ips(msg), IPLONG * sizeof(char));
				sendMessageWithAdd(msg);
				fprintf (stdout, "Asking for connection\n");
				last = getNeighbour(ipa);
				fprintf (stdout, "New last after receiving I_AM_ALIVE message : %d\n", last);
				state = IDLE;
				critSectionRequest();
				sortie++;
				break;
			}
			else if (type(msg) == REQUEST) {}
			else
				handleMessage(msg);
		}
	}

	// No pred is alive, broadcast SEARCH_PREV
	fcntl(this_site.sdRecv, F_SETFL, flags);

	fprintf (stdout, "No alive predecessor found, looking for other sites\n");
	if (!sortie) {
		msg_t min;

		type(msg) = SEARCH_PREV;
		pos(msg) = position;
		fprintf (stdout, "Broadcasting SEARCH_PREV.\n");
		broadcast(msg);

		fcntl(this_site.sdRecv, F_SETFL, flags2);

		timeStart = time(&timeStart);
		timeCur = time(&timeCur);

		// Wait for ACK_SEARCH_PREV
		while(timeCur - timeStart < (2*TMESG)) {
			timeCur = time(&timeCur);
			memset (&msg, 0, SIZE);

			if(recvMessage(&msg, NULL) == -1) 
				continue;

			pos(min) = -1;
			if (type(msg) == ACK_SEARCH_PREV) {
				if (pos(min) == -1)
					memcpy (&min, &msg, SIZE);
				else if (pos(min) < pos(msg))
					memcpy (&min, &msg, SIZE);
				fprintf (stdout, "Got an answer.\n");
			}
			else if (type(msg) == REQUEST) {}
			else
				handleMessage(msg);
		}

		fcntl(this_site.sdRecv, F_SETFL, flags);

		// No answers, regenerate the TOKEN
		if (pos(min) == -1) {
			fprintf (stdout, "No answers, regenerating TOKEN\n");
			tokenPresent = 1;
			last = -1;
			takeCriticalSection();
			sortie ++;
		}

		// Got an answer, ask for connection
		if (!sortie) {
			fprintf (stdout, "Asking for connection\n");
			type(msg) = CONNECTION;
			strncpy(ip(msg), ips(min), IPLONG * sizeof(char));
			last = getNeighbour((unsigned long int)inet_addr(ips(min)));
			sendMessageWithAdd(msg);
			state = IDLE;
			critSectionRequest();
		}
	}
}
//}}}

//{{{ init_structures
int init_structures() {
	int i;
	last = -1;
	next = -1;
	ch_pid = 0;
	state = IDLE;
	tokenPresent = 0;

	position = -1;
	pthread_mutex_init(&mut_check, NULL);
	check = 0;
	acces = 0;
	for (i=0; i<TOLERANCE+1; memset(&predec[i], 0, sizeof(struct sockaddr_in)), i++);

	/* HELLO message broadcasting */
	msg_t broad;
	memset(&broad, 0, SIZE);
	type(broad) = HELLO;
	broadcast(broad);

	waitForHellorep(WAITING_PERIOD);

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

		// Arms a timer
		time_t timeStart, timeCur;
		int flags = fcntl(this_site.sdRecv, F_GETFL);
		int flags2 = flags | O_NONBLOCK;
		fcntl(this_site.sdRecv, F_SETFL, flags2);

		timeStart = time(&timeStart);
		timeCur = time(&timeCur);

		// Wait for an answer
		fprintf (stdout, "Waiting for COMMIT.\n");
		while(timeCur - timeStart < (2*TMESG)) {
			timeCur = time(&timeCur);

			if(recvMessage(&msg, NULL) == -1) 
				continue;

			if (type(msg) == COMMIT)
				return handleCommit(msg);
			else {
				// TODO: Work on reaction when receiving request while waiting commit
				if (type(msg) == REQUEST)
					fprintf (stdout, "Received a REQUEST instead of a COMMIT.\n");
				else if (type(msg) == TOKEN)
					return handleToken(msg);
				else
					handleMessage(msg);
			}
		}

		// If receive no answers
		fprintf (stdout, "I didn't get any COMMIT, looking for the queue...\n");
		fcntl(this_site.sdRecv, F_SETFL, flags);

		memset (&msg, 0, SIZE);
		type(msg) = SEARCH_QUEUE;
		nb_acc(msg) = acces;
		// Broadcast Search_Queue
		fprintf (stdout, "Broadcasting SEARCH_QUEUE.\n");
		broadcast(msg);

		msg_t max;
		pos(max) = -1;

		fcntl(this_site.sdRecv, F_SETFL, flags2);

		timeStart = time(&timeStart);
		timeCur = time(&timeCur);

		// Wait for ACK_SEARCH_QUEUE
		fprintf (stdout, "Expecting ACK_SEARCH_QUEUE.\n");
		while(timeCur - timeStart < 2*TMESG) {
			timeCur = time(&timeCur);
			memset (&msg, 0, SIZE);

			if(recvMessage(&msg, NULL) == -1) 
				continue;

			switch(type(msg)) {
				case ACK_SEARCH_QUEUE:
					fprintf (stdout, "Got an ACK_SEARCH_QUEUE.\n");
					// Save greatest position in the queue
					if (pos(msg) > pos(max)) {
						fprintf (stdout, "New position in QUEUE : %d.\n", pos(msg));
						memcpy(&max, (void *) &msg, SIZE);
					}
					break;
					// Another site discovers the failure
				case SEARCH_QUEUE:
					if (isMe(ips(msg)))
						continue;
					fprintf (stdout, "Another site discovered the failure.\n");
					// The other site hasn't priority, just continue
					if (nb_acc(msg) > acces) 
						continue;
					// Both of sites have the same numbers of access, defines priority with ip
					if (nb_acc(msg) == acces) {
						char *ip_pers = inet_ntoa(this_site.neighbours[0].sin_addr);
						if (strcmp(ip_pers, ips(msg)) <= 0)
							continue;
					}
					// If the other has the priority, just change last and ask it for CS
					unsigned long int ipa = (unsigned long int) inet_addr(ips(msg));
					last = getNeighbour(ipa);
					fprintf (stdout, "I don't have priority, changing my last (%d) and asking for CS again.\n", last);
					state = IDLE;
					return critSectionRequest();
					// TODO: Request processing
				case REQUEST:
					break;
				default:
					handleMessage(msg);
					break;
			}
		}

		fprintf (stdout, "Waiting time passed.\n");

		fcntl(this_site.sdRecv, F_SETFL, flags);

		// No other site in the queue, juste regenerate TOKEN
		if (pos(max) < 0) {
			fprintf (stdout, "No other sites in the queue, regenerate TOKEN (last = -1)\n");
			tokenPresent = 1;
			takeCriticalSection();
		}
		else {
			// Ask for connection to the site with the highest position
			unsigned long int ipa = (unsigned long int) inet_addr(ip(max));
			last = getNeighbour(ipa);
			fprintf (stdout, "Ask for Connection, new last = %d\n", last);
			strncpy(ip(msg), ips(max),IPLONG *sizeof(char));
			if (next(max)) {
				type(msg) = CONNECTION;
				if (sendMessageWithAdd(msg) == -1)
					return -1;
			}
			state = IDLE;
			critSectionRequest();
		}
	}
	else
		fprintf (stderr, "======> Last = -1, but don't have TOKEN <======\n");
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
				fprintf (stdout, "Got a request, but expecting the TOKEN. Sending COMMIT to %d.\n", next);
				type(msg) = COMMIT;
				pos(msg) = position + 1;
				int j;
				for (j=0; j<TOLERANCE; memcpy(pred(msg)+j,predec+j,sizeof(struct sockaddr_in)), j++);
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
			position = -1;
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

//{{{ handleCommit
int handleCommit (msg_t msg) {
	unsigned long int ipa = (unsigned long int)inet_addr(ips(msg));
	pthread_t thread_id;

	fprintf (stdout, "Got a COMMIT, creating a thread to verify validity of predecessor\n");

	if(!getNeighbour(ipa))
		return 0;

	position = pos(msg);
	last = -1;
	int i;
	fprintf (stdout,"Copy the predecessor adress contained in received COMMIT.\n");
	for (i=0; i<TOLERANCE; predec[i+1] = pred(msg)[i], i++);

	inet_aton(ips(msg),&(predec[0].sin_addr));

	if(pthread_create(&thread_id, NULL, (void*)(checkNeighbour), (void*)predec) != 0)
		fprintf(stderr, "Thread creation failure.\n");


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

//{{{ handleAreYouAlive
int handleAreYouAlive(msg_t msg){
	type(msg) = I_AM_ALIVE;
	fprintf (stdout, "ARE_YOU_ALIVE received, sending I_AM_ALIVE\n");
	strncpy(ip(msg), ips(msg), IPLONG * sizeof(char));
	return (sendMessageWithAdd(msg));
}
//}}}

//{{{ handleSearchPrev
int handleSearchPrev (msg_t msg){
	fprintf (stdout, "SEARCH_PREV received.");
	if (position < pos(msg)) {
		fprintf (stdout, "Sending ACK_SEARCH_PREV.\n");
		type(msg) = ACK_SEARCH_PREV;
		pos(msg) = position;
		strncpy(ip(msg), ips(msg), IPLONG * sizeof(char));
		return (sendMessageWithAdd(msg));
	}
	fprintf (stdout, "\n");
	return 0;
}
//}}}

//{{{ handleSearchQueue
int handleSearchQueue(msg_t msg){
	fprintf (stdout, "SEARCH_QUEUE received.");
	if (position >= 0) {
		fprintf (stdout, "Sending ACK_SEARCH_QUEUE.\n");
		type(msg) = ACK_SEARCH_QUEUE;
		pos(msg) = position;
		strncpy (ip(msg), ips(msg), IPLONG * sizeof(char));
		return sendMessageWithAdd(msg) == -1;
	}
	fprintf (stdout, "\n");
	return 0;
}
//}}}

//{{{ handleIAmAlive
int handleIAmAlive(msg_t m) {
	fprintf (stdout, "I_AM_ALIVE received.\n");
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
	fprintf(stdout, "Entering CS\n");
	state = WORKING;
	position = 0;
	int i;
	fprintf (stdout, "Reinitializing predecessor\n");
	for (i=0; i<=TOLERANCE; memset(predec + i++, 0, sizeof(struct sockaddr_in)));

	/* exec CS */
	pthread_t thread_id;
	if(pthread_create(&thread_id, NULL, (void*)(liberation), (void*)20) != 0)
		fprintf(stderr, "Thread creation failure.\n");

	ch_pid = thread_id;

	return 0;
}
//}}}

//{{{ liberation
void liberation(void* arg) {
	pthread_detach(pthread_self());

	sleep((long int)arg);

	state = IDLE;
	if(next != -1) {
		fprintf (stdout, "Sending TOKEN.\n");
		msg_t mes;
		type(mes) = TOKEN;
		if(sendMessage(next, mes) == -1) 
			fprintf(stderr, "======> Error while sending message <====== \n");
		next = -1;
		tokenPresent = 0;
		position = -1;
	}
	else
		last = -1;
	ch_pid = 0;
	fprintf(stdout, "CS released : %d access\n", ++acces);
}
//}}}

//{{{ checkNeighbour
void checkNeighbour (void *arg) {
	pthread_detach(pthread_self());

	fprintf(stdout, "Checking for predecessor viability.\n");

	sigset_t block;
	sigemptyset (&block);
	sigaddset(&block, SIGUSR1);
	sigprocmask(SIG_BLOCK, &block, NULL);

	struct sockaddr_in *pre = (struct sockaddr_in *)arg;
	int failure = 0;

	fprintf (stdout, "Sending ARE_YOU_ALIVE.\n");
	msg_t msg;
	type(msg) = ARE_YOU_ALIVE;
	char *tmpmes;
	tmpmes = inet_ntoa(pre->sin_addr);
	memcpy (ip(msg), tmpmes, IPLONG * sizeof(char));

	while (state == WAITING && !failure) {
		pthread_mutex_lock(&mut_check);
		if (check) {
			fprintf (stderr, "Site Failure Detected\n");
			pthread_mutex_unlock(&mut_check);
			kill(getpid(), SIGUSR1);
			failure ++;
		}
		else{
			check --;
			pthread_mutex_unlock(&mut_check);

			fprintf (stdout, "Sending ARE_YOU_ALIVE.\n");
			if (sendMessageWithAdd(msg) == -1)
				continue;

			sleep(2*TMESG);
		}
	}
}
//}}}

//}}}

//{{{ isMe
int isMe (char *ips) {
	return strcmp(ips, inet_ntoa(this_site.neighbours[0].sin_addr)) ? 0 : 1;
}
//}}}

