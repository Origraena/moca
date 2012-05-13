#include <iostream>
#include <fstream>
#include <math.h> // pour cos() et sin()
#include <limits.h>


#define PI 3.14159265

using namespace std;

int const n=100000;


long long determinant(int x1, int y1, int x2, int y2)
{
	return x1 * y2 - x2 * y1;
}

long long produitScalaire(int x1, int y1, int x2, int y2)
{
	return x1 * x2 + y1 * y2;
}

int xVecteur(int x1, int x2)
{
	return x2 - x1;
}

int yVecteur(int y1, int y2)
{
	return y2 - y1;
}

double distanceEuc(int A[2], int B[2])
{
	return sqrt((double)(A[0]-A[1])*(double)(A[0]-A[1])+(double)(B[0]-B[1])*(double)(B[0]-B[1]));
}

void AffichagePoints(int n, int sommet[][2]){
	//Affichage de n points dont les coordonnees sont donnees dans sommet[n][2]
	//Un fichier Points.ps est cree, visualisable avec un afficheur postscript (ex: ggv, kghostview...)
	ofstream output;
	output.open("Points.ps");//
	output << "%!PS-Adobe-3.0" << endl;
	output << endl;  
	for(int i=0;i<n;i++){
		output << sommet[i][0] << " " << sommet[i][1] << " 2 0 360 arc" <<endl;
		output << "0 setgray" <<endl;
		output << "fill" <<endl;
		output << sommet[i][0]+2 << " " << sommet[i][1] << " moveto" <<endl;
		output << "/Courier findfont 15 scalefont setfont" << endl;
		output << "("<< i << ")" << " show" << endl;
		output << "stroke" << endl;
		output << endl;
	}
	output << endl;
	output.close();
}

void AffichageEnvConv(int n, int sommet[][2], int envconv[]){
	//Affichage de n points dont les coordonnees sont donnees dans sommet[n][2] et de leur
	//enveloppe convexe donnee sous forme de liste cyclique: un tableau de taille n+1 listant les
	//indices des sommets de l'enveloppe convexe. Le premier sommet est repete en fin de liste et le
	//tableau est complete par la valeur -1.
	//Un fichier EnvConv.ps est cree
	ofstream output;
	output.open("EnvConv.ps");//
	output << "%!PS-Adobe-3.0" << endl;
	output << endl;  
	for(int i=0;i<n;i++){
		output << sommet[i][0] << " " << sommet[i][1] << " 2 0 360 arc" <<endl;
		output << "0 setgray" <<endl;
		output << "fill" <<endl;
		output << sommet[i][0]+2 << " " << sommet[i][1] << " moveto" <<endl;
		output << "/Courier findfont 15 scalefont setfont" << endl;
		output << "("<< i << ")" << " show" << endl;
		output << "stroke" << endl;
		output << endl;
	}
	output << endl;
	int i=0;
	while(envconv[i+1]!=-1){
		output << sommet[envconv[i]][0] << " " << sommet[envconv[i]][1] 
 	    << " moveto"  << endl;
		output << sommet[envconv[i+1]][0] << " " << sommet[envconv[i+1]][1] 
 	    << " lineto"  << endl;
		output << "stroke" << endl;
		output << endl;
		i++;
	}
    output.close();
}


//Generation au hasard de l'ensemble des points consideres
void PointAuHasard(int n,int sommet[][2])
{
	int i, rayon = 250, centre[2] = {300, 400};
	
	for(i = 0 ; i < n ; i++)
	{
		sommet[i][0] = rayon + centre[0] + 1;
		sommet[i][1] = rayon + centre[1] + 1;
		while(pow(sommet[i][0]-centre[0], 2)+pow(sommet[i][1]-centre[1], 2) > rayon*rayon)
		{
			sommet[i][0] = rand() % (2*rayon) + centre[0] - rayon;
			sommet[i][1] = rand() % (2*rayon) + centre[1] - rayon;
		}
	}
}

//Renvoie Vrai si p2 est strictement a droite de la droite p_0p_1
bool AnglePolaireInferieur(int p0[2], int p1[2], int p2[2])
{
	if(determinant(p1[0]-p0[0], p1[1]-p0[1], p2[0]-p0[0], p2[1]-p0[1]) < 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

//Parcours de Jarvis
void Jarvis(int n, int sommet[][2],int envconv[])
{
	int i, pMin = 0, pCourant, indEnv = 0, x1, y1, x2, y2;
	long long p, pSuivant, det, prod;
	
	for(i = 1 ; i < n ; i++)
	{
		if(sommet[i][1] < sommet[pMin][1])
			pMin = i;
	}
	pCourant = pMin;
	envconv[indEnv] = pCourant;
	indEnv++;
	
	do
	{
		cout << "pCourant : " << pCourant << endl;
		if(pCourant == 0)
			p = 1;
		else
			p = 0;
		pSuivant = p;
		
		for(i = 0 ; i < n ; i++)
		{
			if(i != pCourant && i != p)
			{
				x1 = xVecteur(sommet[pCourant][0], sommet[p][0]);
				y1 = yVecteur(sommet[pCourant][1], sommet[p][1]);
				x2 = xVecteur(sommet[pCourant][0], sommet[i][0]);
				y2 = yVecteur(sommet[pCourant][1], sommet[i][1]);
				// déterminant de [pCourant:p] ; [pCourant;i]
				det = determinant(x1, y1, x2, y2);
				
				// produit scalaire de [pCourant;p] ; [pCourant;i]
				prod = produitScalaire(x1, y1, x2, y2);
				
				if(det <= 0)
				{
					if(AnglePolaireInferieur(sommet[pCourant], sommet[pSuivant], sommet[i]))
					{
						pSuivant = i;
					}
				}
			}
		}
		pCourant = pSuivant;
		
		envconv[indEnv] = pCourant;
		indEnv++;
	} while (pCourant != pMin);
}

int main()
{
	srand(time(NULL));
	int sommet[n][2];
	int envconv[n+1];
	
	for(int i=0;i<n+1;i++)
	{
		envconv[i]=-1;
	}
	
	PointAuHasard(n,sommet);
	AffichagePoints(n,sommet);
	Jarvis(n,sommet,envconv);
	AffichageEnvConv(n,sommet,envconv);
	
	return 0;
}
