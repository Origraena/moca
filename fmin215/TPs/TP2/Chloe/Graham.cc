
#include <iostream>
#include <fstream>
#include <math.h> // pour cos() et sin()

#define PI 3.14159265

using namespace std;

int const n=20;


long long determinant(int x1, int y1, int x2, int y2)
{
	return x1 * y2 - x2 * y1;
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
    for(int i=0;i<n;i++){
		if(envconv[i+1]!=-1){
			output << sommet[envconv[i]][0] << " " << sommet[envconv[i]][1] 
			<< " moveto"  << endl;
			output << sommet[envconv[i+1]][0] << " " << sommet[envconv[i+1]][1] 
			<< " lineto"  << endl;
			output << "stroke" << endl;
			output << endl;
		}
    }
    output.close();
}


//Renvoie Vrai si p2 est strictement a droite de la droite oriente p_0p_1
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



void TriRapidePolaire(int min, int n, int sommet[][2], int t, int TriPolaire[]){
	// En entree, les parametres sont min: l'indice du sommet autour duquel les autres sont tries par ordre polaire croissant
	//n: le nombre de sommet
	//TriPolaire le tableau contenant la liste des indices des sommets a trier, donc au debut, tous sauf min
	//t: la taille de TriPolaire
	// donc, a l'appel initial, t vaut n-1.
	// L'algo modifie TriPolaire et le retourne trie par angle polaire croissant par rapport au sommet 
	// d'indice min.
	if(t>1){
		int pivot=TriPolaire[0]; //indice du sommet pivot
		int d=0; // nombre de sommets a droite de min_pivot
		for(int i=1;i<t;i++){//On compte (on ne tient pas compte de pivot)
			if(AnglePolaireInferieur(sommet[min],sommet[pivot],sommet[TriPolaire[i]])){d++;}
		}
		int Tg[t-d-1];
		int Td[d];
		int indice_gauche=0; //indice d'insertion dans Tg
		int indice_droit=0; //indice d'insertion dans Td
		
		for(int i=1;i<t;i++){ //Remplissage de Tg et Td (on ne tient pas compte de pivot)
			if(AnglePolaireInferieur(sommet[min],sommet[pivot],sommet[TriPolaire[i]])){//A droite
				Td[indice_droit]=TriPolaire[i];
				indice_droit++;}
			else{//A gauche
				Tg[indice_gauche]=TriPolaire[i];
				indice_gauche++;}}
		
		//On trie Td et Tg
		if(d>0){TriRapidePolaire(min,n,sommet,d,Td);}
		if(t-d-1>0){TriRapidePolaire(min,n,sommet,t-d-1,Tg);}
		//Fusion dans TriPolaire
		for(int i=0;i<t;i++){
			if(i<d){TriPolaire[i]=Td[i];}
			if(i==d){TriPolaire[i]=pivot;}
			if(i>d){TriPolaire[i]=Tg[i-d-1];}
		}
	}
}



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

void Graham(int n, int sommet[][2], int envconv[])
{
	// PHASE 1
	int p0 = 0, i;
	int TriPolaire[n-1];
	
	for(i = 1 ; i < n ; i++)
	{
		if(sommet[i][1] < sommet[p0][1])
			p0 = i;
		else if(sommet[i][1] == sommet[p0][1])
		{
			if(sommet[i][0] < sommet[p0][0])
				p0 = i;
		}
	}
	
	printf("Sommet min : %d\n", p0);
	
	for(i = 0 ; i < n-1 ; i++)
	{
		TriPolaire[i] = i;
	}
	if(p0 != n-1)
		TriPolaire[p0] = n-1;
	
	TriRapidePolaire(p0, n, sommet, n-1, TriPolaire);
	
	printf("Tri polaire : ");
	for(i = 0 ; i < n-1 ; i++)
	{
		printf("%d ", TriPolaire[i]);
	}
	printf("\n");
	
	// PHASE 2
	int indiceEnv = 3;
	envconv[0] = p0;
	printf("Empile %d\n", envconv[0]);
	envconv[1] = TriPolaire[0];
	printf("Empile %d\n", envconv[1]);
	envconv[2] = TriPolaire[1];
	printf("Empile %d\n", envconv[2]);
	for(i = 2 ; i < n-1 ; i++)
	{
		while (AnglePolaireInferieur(sommet[envconv[indiceEnv-2]], sommet[envconv[indiceEnv-1]], sommet[TriPolaire[i]]))
		{
			indiceEnv--;
			printf("Depile %d\n", envconv[indiceEnv]);
		}
		envconv[indiceEnv] = TriPolaire[i];
		printf("Empile %d\n", envconv[indiceEnv]);
		indiceEnv++;
	}
	envconv[indiceEnv] = p0;
}


int main()
{
	srand(time(NULL));
	int sommet[n][2];
	int envconv[n+1];
	for(int i=0;i<n+1;i++){envconv[i]=-1;}
	PointAuHasard(n,sommet);
	AffichagePoints(n,sommet);
	Graham(n,sommet,envconv);
	AffichageEnvConv(n,sommet,envconv);
}


