#include <iostream>
#include <fstream>
#include <math.h> // pour cos() et sin()
#define PI 3.14159265

using namespace std;

int const n=10;

/*******************************************************************/
// Structure triangle
/*******************************************************************/

//structure triangle, pour une variable triangle  T, ses sommets sont accessibles par
//T.a, T.b et T.c

typedef struct {
  int a;
  int b;
  int c;
} triangle;

/*******************************************************************/
//Petites fonctions arithmetiques ou geometriques
/*******************************************************************/


int det(int p0[2], int p1[2], int p2[2],int p3[2]){
//Renvoie le determinant de p0p1,p2p3
  return (p1[0]-p0[0])*(p3[1]-p2[1])-(p1[1]-p0[1])*(p3[0]-p2[0]);
}

int ProduitScalaire(int p0[2], int p1[2], int p2[2],int p3[2]){
//Renvoie le produit scalaire de p0p1,p2p3
  return (p1[0]-p0[0])*(p3[0]-p2[0])+(p1[1]-p0[1])*(p3[1]-p2[1]);
}

int Carre(int x){
  //Sans commentaire
  return x*x;}

int NormeAuCarre(int p1[2],int p2[2]){
  //Calcul la norme au carre de p1p2
  return Carre(p1[1]-p2[1])+Carre(p1[0]-p2[0]);
}

bool EstADroite(int A[2], int B[2], int C[2]){
  //Renvoie vrai si C est strictement a droite de (AB) oriente de A vers B.
  if(det(A,B,A,C)>=0){return false;}
  else{return true;}
}

/*******************************************************************/
//Generation de sommets au hasard
/*******************************************************************/

void PointAuHasard(int n,int sommet[][2]){
  //Tire n points au hasard uniformement repartis dans un disque, leurs coordonnees sont
  //stockees dans sommet[][2]
  srand(time(NULL));
  for(int i=0;i<n;i++){
    int r= rand()%250;
    int theta= rand() %360;
    sommet[i][0]=(int) (300 +r*cos( (float)theta /180.0 * PI ));
    sommet[i][1]=(int) (400 + r*sin((float)theta /180.0 * PI ));
  }
}

/*******************************************************************/
//Fonctions d'affichage
/*******************************************************************/

void AffichagePoints(int n, int sommet[][2]){
  //Affichage de n points dont les coordonnees sont dans sommet[n][2]
  //Un fichier Points.ps est cree, visualisable avec un afficheur postscript (ex: ggv, kghostview)
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

void AffichageTriangulation(int n, int sommet[][2], int t, triangle T[]){
  //Affichage des n points du plan dont les coordonnees sont dans sommet[][2] et d'une triangulation
  //en t triangles stockes dans T, tableau de taille t.
  //Produit en sortie un fichier Triangulation.ps
   ofstream output;
   output.open("Triangulation.ps");//
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
    for(int i=0;i<t;i++){
      output << sommet[T[i].a][0] << " " << sommet[T[i].a][1] 
 	    << " moveto"  << endl;
      output << sommet[T[i].b][0] << " " << sommet[T[i].b][1]
 	    << " lineto"  << endl;
      output << sommet[T[i].b][0] << " " << sommet[T[i].b][1] 
 	    << " moveto"  << endl;
      output << sommet[T[i].c][0] << " " << sommet[T[i].c][1]
 	    << " lineto"  << endl;
      output << sommet[T[i].c][0] << " " << sommet[T[i].c][1] 
 	    << " moveto"  << endl;
      output << sommet[T[i].a][0] << " " << sommet[T[i].a][1]
 	    << " lineto"  << endl;
      output << "stroke" << endl;
      output << endl;
    }
    output.close();
}


/************************************************************************/
//Calcul de la triangulation
/************************************************************************/




void TriLexicographique(int n, int sommet[][2], int t, int Tri[]){
  //En entree, Tri, tableau de taille n, contient les indices des sommets a trier, a l'initialisation
  //Tri[i]=i, et t est la taille de Tri, donc t=n pour le premier appel.
  //En sorti, Tri contient les indices des sommets tries par ordre lexicographique croissant. 
  if(t>1){
    int pivot=Tri[0]; //indice du sommet pivot
    int d=0; // nombre de sommets a droite de min_pivot
    for(int i=1;i<t;i++){//On compte (on ne tient pas compte de pivot)
      if((sommet[Tri[0]][0]<sommet[Tri[i]][0])||
	 ((sommet[Tri[0]][0]==sommet[Tri[i]][0])&&
	  (sommet[Tri[0]][1]<sommet[Tri[i]][1]))){d++;}
    }
    int Tg[t-d-1];
    int Td[d];
    int indice_gauche=0; //indice d'insertion dans Tg
    int indice_droit=0; //indice d'insertion dans Td
    for(int i=1;i<t;i++){ //Remplissage de Tg et Td (on ne tient pas compte de pivot)
      if((sommet[Tri[0]][0]<sommet[Tri[i]][0])||
	 ((sommet[Tri[0]][0]==sommet[Tri[i]][0])&&
	  (sommet[Tri[0]][1]<sommet[Tri[i]][1]))){//A droite
      Td[indice_droit]=Tri[i];
      indice_droit++;}
      else{//A gauche
      Tg[indice_gauche]=Tri[i];
      indice_gauche++;}}

    //On trie Td et Tg
    if(d>0){TriLexicographique(n,sommet,d,Td);}
    if(t-d-1>0){TriLexicographique(n,sommet,t-d-1,Tg);}
    //Fusion dans Tri
    for(int i=0;i<t;i++){
      if(i<t-d-1){Tri[i]=Tg[i];}
      if(i==t-d-1){Tri[i]=pivot;}
      if(i>t-d-1){Tri[i]=Td[i-t+d];}
    }
  }
}


int TriangulIncrementale(int n, int sommet[][2], int tri[], triangle T[]){
  //En sortie, T doit contenir la liste des triangles de la triangulation incrementale.
  //Le nombre de triangles crees est retournee.

  //Tableau gerant les sommets de l'enveloppe convexe courante
  //taille_envconv est la taille effective de l'enveloppe convexe
  //Le premier sommet de envconv est le dernier sommet insere, les autres suivent dans le sens direct
  //Vous etes libre d'utiliser une autre structure de donnees qu'un (pauvre) tableau.

  int envconv[n];
  int taille_envconv=3;
  
  //Creation du premier triangle de sommets tri[0], tri[1] et tri[1].

  //
  //A COMPLETER
  //


  //Deroulement de l'algorithme
  for(int i=3;i<n;i++){//insertion de tri[i]
  
    //Recherche de khaut, l'indice du sommet le plus loin dans le sens direct visible depuis tri[i]
   
    //
    //A COMPLETER
    //

    //Recherche de kbas, l'indice du sommet le plus loin dans le sens indirect visible depuis tri[i]
   
    //
    //A COMPLETER
    //
 
    //Mise a jour de envconv, on peut utiliser une copie temporaire
   
    //
    //A COMPLETER
    //

}

/*******************************************************************/
//main
/*******************************************************************/


int main(){
  int sommet[n][2];
  int tri[n];
  triangle T[3*n];
  for(int i=0;i<n;i++){tri[i]=i;}
  PointAuHasard(n,sommet);
  AffichagePoints(n,sommet);
  TriLexicographique(n,sommet,n,tri);
  int t=TriangulIncrementale(n,sommet,tri,T);
  AffichageTriangulation(n,sommet,t,T);
}

