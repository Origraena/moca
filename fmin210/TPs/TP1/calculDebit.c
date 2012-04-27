
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define CONVERSION 1000

char unito[3][20] = {"o/s", "ko/s", "mo/s"};
char unitb[3][20] = {"b/s", "kb/s", "mb/s"};

typedef struct cpt {
	double plus;
	double moins;
	double drop;
	double recept;
	char *nom;
} cpt;

void affiche_conn (cpt connex, int date) {
	int unit = 0, bunit = 0;
	
	//printf ("Tps Exécution : %d sec.\n\n", date);

	printf ("Connexion : %s \n", connex.nom);

	connex.plus /= 1. * date;

	while (connex.plus >= CONVERSION) {
		connex.plus /= CONVERSION;
		unit ++; bunit ++;
	}

	if (connex.plus * 8 >= CONVERSION)
		bunit ++;

	printf("\t* Arrivée : %.2f %s (%.2f %s)\n", connex.plus, unito[unit], bunit > unit ? connex.plus*8/CONVERSION : connex.plus*8, unitb[bunit]);
	unit = 0; bunit = 0;

	connex.moins /= date;

	while (connex.moins >= CONVERSION) {
		connex.moins /= CONVERSION;
		unit ++; bunit ++;
	}

	if (connex.moins * 8 >= CONVERSION)
		bunit ++;

	printf("\t* Départ : %.2f %s (%.2f %s)\n", connex.moins, unito[unit], bunit > unit ? connex.moins*8/CONVERSION : connex.moins*8, unitb[bunit]);
	unit = 0; bunit = 0;

	connex.drop /= date;

	while (connex.drop >= CONVERSION) {
		connex.drop /= CONVERSION;
		unit ++; bunit ++;
	}

	if (connex.drop * 8 >= CONVERSION)
		bunit ++;

	printf("\t* Perte : %.2f %s (%.2f %s)\n", connex.drop, unito[unit], bunit > unit ? connex.drop*8/CONVERSION : connex.drop*8, unitb[bunit]);
	unit = 0; bunit = 0;

	connex.recept /= date;

	while (connex.recept >= CONVERSION) {
		connex.recept /= CONVERSION;
		unit ++; bunit ++;
	}

	if (connex.recept * 8 >= CONVERSION)
		bunit ++;

	printf("\t* Reception : %.2f %s (%.2f %s)\n", connex.recept, unito[unit], bunit > unit ? connex.recept*8/CONVERSION : connex.recept*8, unitb[bunit]);
	unit = 0; bunit = 0;

	printf ("\n");
}

void affiche_stats (cpt connex, int date) {
	int unit = 0, bunit = 0;

	connex.moins /= date;

	while (connex.moins >= CONVERSION) {
		connex.moins /= CONVERSION;
		unit ++; bunit ++;
	}

	if (connex.moins * 8 >= CONVERSION)
		bunit ++;

	printf("\t* Debit moyen : %.2f %s (%.2f %s)\n", connex.moins, unito[unit], bunit > unit ? connex.moins*8/CONVERSION : connex.moins*8, unitb[bunit]);
	
	
	printf("\t* Taux d'arrivee : %.2f\n",
	       (double)connex.recept/(double)connex.plus);
	printf("\t* Taux de pertes : %.2f\n",
	       1 - ((double)connex.recept/(double)connex.plus));
	printf ("\n");
}

int main(int argc, char* argv[], char* env[]) {
	if(argc < 2)
		return -1;
	FILE* fTraces = fopen(argv[1], "r");
	if (!fTraces)
		printf ("Dans ton cul.\n");
	cpt udp = {0,0,0,0, "udp"};
	cpt tcp1 = {0,0,0,0, "tcp1"};
	cpt tcp2 = {0,0,0,0, "tcp2"};
	cpt tcp3 = {0,0,0,0, "tcp3"};
	cpt total = {0,0,0,0, "Total"};
	cpt* connection;

	char operation, ack;
	double os, time;
	int src, dest, taille, osef, prot;
	
	while(fscanf(fTraces, "%c %lf %d %d %c%c%c %d ---%c--- %d %lf %lf %d %d\n", &operation, &time, &src, &dest, &ack, &osef, &osef, &taille, &osef, &prot, &os, &os, &osef, &osef) != EOF) {
		if (ack != 'a')
		{
		switch(prot) {
			case 1:
				connection = &udp;
				break;
			case 2:
				connection = &tcp1;
				break;
			case 3:
				connection = &tcp2;
				break;
			case 4:
				connection = &tcp3;
				break;
		}
		switch(operation) {
			case '+':
				connection->plus+=taille;
				total.plus+=taille;
				break;
			case '-':
				connection->moins+=taille;
				total.moins+=taille;
				break;
			case 'd' :
				connection->drop+=taille;
				total.drop+=taille;
				break;
			case 'r' :
				connection->recept+=taille;
				total.recept+=taille;
				break;
		}
		}
	}

	int atime = (int) ceil(time);

	affiche_conn(udp,  atime);
	affiche_stats(udp,  atime);
	affiche_conn(tcp1, atime);
	affiche_stats(tcp1,  atime);
	affiche_conn(tcp2, atime);
	affiche_stats(tcp2,  atime);
	affiche_conn(tcp3, atime);
	affiche_stats(tcp3,  atime);

	affiche_conn(total, atime);
	affiche_stats(total,  atime);
	
	fclose(fTraces);
	
	return 0;
}



