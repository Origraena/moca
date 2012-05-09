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
#include <signal.h>

#include "networkUtils.h"
#include "utils.h"
#include "partition.h"

#define WAITING_PERIOD 5
#define TMESG					 3


int last;
int next;
int tokenPresent;
int ch_pid;
site_state state;

// Only needed for faul tolerant extension
int position;
int check;
int acces;
pthread_mutex_t mut_check;
struct sockaddr_in predec[TOLERANCE+1];

int init_structures();
int critSectionRequest();
int handleMessage(msg_t mes);
int handleRequest(msg_t mes);
int handleToken(msg_t mes);
int takeCriticalSection();
void liberation(void* arg);
int handleHello(msg_t mes);
int handleResource(msg_t mes);
int handleSolution(msg_t mes);
int handleHelloRep(msg_t mes, struct sockaddr_in* netParamsNeighbour);
int waitForHellorep(int waitingPeriod);

extern struct problem this_problem;
extern int fileW;
extern int _verbose;

#endif
