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


typedef enum site_state
{
	IDLE = 1,
	WAITING = 2,
	WORKING = 3
} site_state;

int last;
int next;
int tokenPresent;
site_state state;

int init_structures();
int critSectionRequest();
int handleMessage(int type, char* message);
int handleRequest(char* ip);
int handleToken(char* message);
int takeCriticalSection();
void execCS();
int handleHelloRep(char* message);
int waitForHellorep(int waitingPeriod);


#endif
