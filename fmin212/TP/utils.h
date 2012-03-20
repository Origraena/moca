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

typedef enum msg_type {
	MESSAGE = 1,
	REQUEST = 2,
	TOKEN = 3
} msg_type;


void itoa(long n, char** res);

#endif
