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

#define TOLERANCE 5
#define IPLONG    16

// {{{ Data Structures
typedef enum msg_type {
	MESSAGE = 1,
	REQUEST = 2,
	TOKEN = 3,
	HELLO = 4,
	HELLOREP = 5,
	COMMIT = 8,
	ARE_YOU_ALIVE = 16,
	I_AM_ALIVE = 32,
	SEARCH_PREV = 64,
	SEARCH_QUEUE = 128,
	ACK_SEARCH_QUEUE = 256
} msg_type;

// Attempts to represent all kind of messages used in fault tolerant extensions
// to implements only one message structure
typedef struct message {
	msg_type _typ_mes;
	int _id_sender;
	int _pos_next_queue; // if msg_type == SEARCH_QUEUE, it is used to store number of access to cs
	int _pred[TOLERANCE];
	char _ip[IPLONG];
} msg_t;
// }}}

// {{{ Macros
#define id(m) m._id_sender
#define pos(m) m._pos_next_queue
#define pred(m) m._pred
#define type(m) m._typ_mes
#define ip(m) m._ip
// }}}

void itoa(long n, char** res);

#endif
