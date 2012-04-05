//
//  utils.h
//  
//
//  Created by Chloe Desdouits on 20/03/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef _utils_h
#define _utils_h

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>

#define TOLERANCE 5
#define IPLONG    16
#define SIZE 5*sizeof(int)+2*IPLONG*sizeof(char)+TOLERANCE*sizeof(struct sockaddr_in)

// {{{ Data Structures
typedef enum msg_type {
	MESSAGE =						1,
	REQUEST =						2,
	TOKEN =							3,
	HELLO =							4,
	HELLOREP =					5,
	COMMIT =						6,
	ARE_YOU_ALIVE =			7,
	I_AM_ALIVE =				8,
	SEARCH_PREV =				9,
	SEARCH_QUEUE =		 10,
	ACK_SEARCH_QUEUE = 11,
	CONNECTION =			 12,
	ACK_SEARCH_PREV =  13,
} msg_type;

// Attempts to represent all kind of messages used in fault tolerant extensions
// to implements only one message structure
typedef struct message {
	msg_type _typ_mes;
	char _sender[IPLONG];
	int _pos_next_queue;
	int _nb_access;
	int _has_next;
	int _has_token;
	struct sockaddr_in _pred[TOLERANCE];
	char _ip[IPLONG];
} msg_t;
// }}}

// {{{ Macros
#define ips(m) m._sender
#define pos(m) m._pos_next_queue
#define pred(m) m._pred
#define type(m) m._typ_mes
#define ip(m) m._ip
#define next(m) m._has_next
#define nb_acc(m) m._nb_access
#define tok(m) m._has_token
// }}}

void itoa(long n, char** res);

#endif
