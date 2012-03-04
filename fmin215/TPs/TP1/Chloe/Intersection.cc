#ifndef DEF_INTERSECT
#define DEF_INTERSECT

#include <iostream>
#include <fstream>
#include "ARN.h"

using namespace std;

int const n=10;


//Le tableau des segments: segments[2*i] et segments[2*i+1] sont les 
// extremites du segments i

long long segments[2*n][2];


long long determinant(int x1, int y1, int x2, int y2)
{
	return x1 * y2 - x2 * y1;
}

long long determinant(int v1[2], int v2[2])
{
	return v1[0] * v2[1] - v2[0] * v1[1];
}

long long produitScalaire(int x1, int y1, int x2, int y2)
{
	return x1 * x2 + y1 * y2;
}

bool ClesEgales(int cle1, int cle2){
	if(cle1==cle2){
		return true;
	}
	else{
		return false;}
}

//Renvoie vrai ssi le premier point du segment cle2
//est a gauche du segment cle1 (segment cle1 < segment cle2)

bool CleInferieure(int cle1, int cle2)
{
	
	//Segment p1p2
	long long x1=segments[2*cle1][0];
	long long y1=segments[2*cle1][1];
	long long x2=segments[2*cle1+1][0];
	long long y2=segments[2*cle1+1][1];
	
	//Point p3 du segment p3p4
	long long x3=segments[2*cle2][0];
	long long y3=segments[2*cle2][1];
	
	if(determinant(x2-x1, y2-y1, x3-x1, y3-y1) > 0)
	{
		//cout << cle1 << " inferieure a " << cle2 << endl;
		return true;
	}
	return false;
}




void AffichageSegments(int n, long long segments[][2]){
	//Affichage de n segments dont les coordonnees sont donnees dans segments[n][4]
	//Un fichier Segments.ps est cree, visualisable avec un afficheur postscript (ex: ggv, kghostview...)
	ofstream output;
	output.open("Segments.ps");//
	output << "%!PS-Adobe-3.0" << endl;
	output << endl;
	for(int i=0;i<n;i++){
		output << segments[2*i][0] << " " << segments[2*i][1] << " 2 0 360 arc" <<endl;
		output << "0 setgray" <<endl;
		output << "fill" <<endl;
		
		output << segments[2*i+1][0] << " " << segments[2*i+1][1] << " 2 0 360 arc" <<endl;
		output << "0 setgray" <<endl;
		output << "fill" <<endl;
		
		output << segments[2*i][0] << " " << segments[2*i][1] << " moveto"  << endl;
		output << segments[2*i+1][0] << " " << segments[2*i+1][1] << " lineto"  << endl;
		output << "stroke" << endl;
		
		output << segments[2*i][0]-20 << " " << segments[2*i][1] << " moveto" <<endl;
		output << "/Courier findfont 15 scalefont setfont" << endl;
		output << "("<< i << ")" << " show" << endl;
		output << "stroke" << endl;
		output << endl;
	}
	output << endl;
	
	output.close();
}


//Generation au hasard de l'ensemble des segments consideres
//Le premier sommet est celui d'abscisse minimale, on refusera des segments verticaux:
//Choisir segments[2*i][0] (=x) dans [20,400] et segments[2*i][1] (=y) dans [100,700]
//Choisir segments[2*i+1][0] dans [x+1,x+100] et segments[2*i+1][1] dans [y-50,y+50]

void SegmentsAuHasard(int n, long long segments[][2])
{
	int i;
	
	for(i = 0 ; i < n ; i++)
	{
		segments[2*i][0] = (rand() % 380) + 20;
		segments[2*i][1] = (rand() % 600) + 100;
		
		segments[2*i+1][0] = (rand() % 99) + (segments[2*i][0]+1);
		segments[2*i+1][1] = (rand() % 100) +(segments[2*i][1])-50;
		cout << "Segment " << i << " : (" << segments[2*i][0] << ";" << segments[2*i][1] << ") -> (" << segments[2*i+1][0]<< ";" << segments[2*i+1][1] << ")" << endl;
	}
}


//Renvoie Vrai si les deux segments p1p2 et p3p4 s'intersectent
//Dans un premier temps, on pourra supposer que deux segments ne sont jamais
//portes par une meme droite


bool Intersectent(long long p1[2], long long p2[2], long long p3[2], long long p4[2])
{
	if(determinant(p2[0]-p1[0], p2[1]-p1[1], p3[0]-p1[0], p3[1]-p1[1]) == 0 
	   && 
	   determinant(p3[0]-p1[0], p3[1]-p1[1], p4[0]-p3[0], p4[1]-p3[1]) == 0)
		// si p1, p2, p3 et p4 sont alignes
	{
		if(p1[0] == p2[0]) //s'ils ont la meme abscisse
		{
			if(max(p1[1], p2[1]) < min(p3[1], p4[1])
			   ||
			   max(p3[1], p4[1]) < min(p1[1], p2[1]))
			{
				return true;
			}
		}
		else
		{
			if(max(p1[0], p2[0]) < min(p3[0], p4[0])
			   ||
			   max(p3[0], p4[0]) < min(p1[0], p2[0]))
			{
				return true;
			}
		}
	}
	else // pas alignes
	{
		if(
		   (determinant(p2[0]-p1[0], p2[1]-p1[1], p3[0]-p1[0], p3[1]-p1[1])
			*
			determinant(p2[0]-p1[0], p2[1]-p1[1], p4[0]-p1[0], p4[1]-p1[1])
			<= 0)
		   &&
		   (determinant(p4[0]-p3[0], p4[1]-p3[1], p3[0]-p1[0], p3[1]-p1[1])
			*
			determinant(p4[0]-p3[0], p4[1]-p3[1], p3[0]-p2[0], p3[1]-p2[1])
			<= 0)
		   )
		{
			return true;
		}
	}
	return false;
}


void TriLexicographique(long long segments[][2], int t, int Tri[]){
	//En entree, Tri, tableau de taille 2*n, contient les indices dans 'segments' des sommets a trier, 
	//a l'initialisation Tri[i]=i, et t est la taille de Tri, donc t=2*n pour le premier appel.
	//En sorti, Tri contient les indices des sommets tries par ordre lexicographique croissant. 
	if(t>1){
		int pivot=Tri[0]; //indice du sommet pivot
		int d=0; // nombre de sommets a droite de min_pivot
		for(int i=1;i<t;i++){//On compte (on ne tient pas compte de pivot)
			if((segments[Tri[0]][0]<segments[Tri[i]][0])||
			   ((segments[Tri[0]][0]==segments[Tri[i]][0])&&
				(segments[Tri[0]][1]<segments[Tri[i]][1]))){d++;}
		}
		int Tg[t-d-1];
		int Td[d];
		int indice_gauche=0; //indice d'insertion dans Tg
		int indice_droit=0; //indice d'insertion dans Td
		for(int i=1;i<t;i++){ //Remplissage de Tg et Td (on ne tient pas compte de pivot)
			if((segments[Tri[0]][0]<segments[Tri[i]][0])||
			   ((segments[Tri[0]][0]==segments[Tri[i]][0])&&
				(segments[Tri[0]][1]<segments[Tri[i]][1]))){//A droite
				   Td[indice_droit]=Tri[i];
				   indice_droit++;}
			else{//A gauche
				Tg[indice_gauche]=Tri[i];
				indice_gauche++;}}
		
		//On trie Td et Tg
		if(d>0){TriLexicographique(segments,d,Td);}
		if(t-d-1>0){TriLexicographique(segments,t-d-1,Tg);}
		//Fusion dans Tri
		for(int i=0;i<t;i++){
			if(i<t-d-1){Tri[i]=Tg[i];}
			if(i==t-d-1){Tri[i]=pivot;}
			if(i>t-d-1){Tri[i]=Td[i-t+d];}
		}
	}
}


//Algo d'intersection
//Renvoie vrai si il existe 2 segments qui s'intersectent, faux sinon
//Fonctionne en nlog n

bool Intersection(int n, long long segments[][2])
{
	int i;
	//Tri contient les abscisses des extremites des segments, c'est l'echeancier
	int Tri[2*n];
	for(i = 0 ; i < 2*n ; i++){
		Tri[i]=i;
	}
	
	TriLexicographique(segments,2*n,Tri);
	
	//Un arbre rouge et noir pour gerer l'ordre courant sur les segments
	
	ARN ordre(&ClesEgales,&CleInferieure);
	
    for(i = 0 ; i < 2*n ; i++)
	{
		if((Tri[i] % 2) == 0) // si le point est une extremite gauche
		{
			int numS = Tri[i]/2;
			ordre.Insere(numS);
			int numSuccS = ordre.Successeur(numS);
			int numPredS = ordre.Predecesseur(numS);
			
			if(numPredS != -1 && Intersectent(segments[2*numS], segments[2*numS+1], segments[2*numPredS], segments[2*numPredS+1]))
			{
				cout << "Intersection entre " << numS << " et " << numPredS << endl;
				return true;
			}
			if(numSuccS != -1 && Intersectent(segments[2*numS], segments[2*numS+1], segments[2*numSuccS], segments[2*numSuccS+1]))
			{
				cout << "Intersection entre " << numS << " et " << numSuccS << endl;
				return true;
			}
		}
		else // si le point est une extremite droite
		{
			int numS = (Tri[i] - 1)/2;
			int numSuccS = ordre.Successeur(numS);
			int numPredS = ordre.Predecesseur(numS);
			
			if(numPredS != -1 && numSuccS != -1 && Intersectent(segments[numPredS*2], segments[numPredS*2+1], segments[2* numSuccS], segments[2* numSuccS +1]))
			{
				
				cout << "Intersection entre " << numPredS << " et " << numSuccS << endl;
				return true;
			}
			ordre.Supprime(numS);
		}
    }
	return false;
}

int main()
{
	srand(time(NULL));
	
	//Tirage des segments au hasasd  
	SegmentsAuHasard(n,segments);
	AffichageSegments(n,segments);
	
	for(int i = 0 ; i < n ; i++)
	{
		for(int j = i+1 ; j < n ; j++)
		{
			if(Intersectent(segments[2*i], segments[2*i+1], segments[2*j], segments[2*j+1]))
			{
				cout << "Intersection entre " << i << " et " << j << endl;
			}
		}
	}
	if(Intersection(n,segments))
	{
		cout << "Il y a une intersection." << endl;
	}
	else
	{
		cout <<"Il n'y a pas d'intersection." << endl;
	}
}

#endif
