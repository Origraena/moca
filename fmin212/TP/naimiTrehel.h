//
//  naimiTrehel.h
//  
//
//  Created by Chloe Desdouits on 20/03/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef _naimiTrehel_h
#define _naimiTrehel_h

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "networkUtils.h"
#include "utils.h"

#define WAITING_PERIOD 5


int last;
int next;
int tokenPresent;
site_state state;

// Only needed for faul tolerant extension
int position;
struct sockaddr_in predec[TOLERANCE+1];

int init_structures();
int critSectionRequest();
int handleMessage(msg_t mes);
int handleRequest(msg_t mes);
int handleToken(msg_t mes);
int takeCriticalSection();
void liberation(void* arg);
int handleHello(msg_t mes);
int handleHelloRep(msg_t mes, struct sockaddr_in* netParamsNeighbour);
int waitForHellorep(int waitingPeriod);


#endif
