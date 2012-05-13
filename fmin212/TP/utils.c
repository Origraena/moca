//
//  utils.c
//  
//
//  Created by Chloe Desdouits on 20/03/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "utils.h"


void itoa(long n, char** res) {
	int i = 1, nbChiffres, iDeb = 0;
	long trans = n;
	char* chaine;
	
	while(trans > 10) {
		trans /= 10;
		i++;
	}
	nbChiffres = i;
	
	if(n < 0) {
		nbChiffres++;
		iDeb = 1;
	}
	chaine = malloc((nbChiffres+1) * sizeof(char));
	if(n < 0)
		chaine[0] = '-';
		
	trans = n;
	
	for(i = nbChiffres-1 ; i >= iDeb ; i--) {
		chaine[i] = 48 + abs(trans % 10);
		trans /= 10;
	}
	chaine[nbChiffres] = 0;
	
	*res = chaine;
}


