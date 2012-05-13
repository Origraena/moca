//
//  utils.c
//  
//
//  Created by Chloe Desdouits on 20/03/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "utils.h"
#include "core.h"

void itoa(int n, char** res) {
	int i = 1, nbChiffres, iDeb = 0;
	int trans = n;
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


void ltoa(long n, char** res) {
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



void utoa(uint n, char** res) {
	int i = 1, nbChiffres, iDeb = 0;
	uint trans = n;
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


