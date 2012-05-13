#include <iostream>
#include <fstream>
#include <math.h> // pour cos() et sin()
#define PI 3.14159265

using namespace std;

int const n=60;

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

//-------------------------------

int ProduitScalaire(int p0[2], int p1[2], int p2[2],int p3[2]){

//Renvoie le produit scalaire de p0p1,p2p3

  return (p1[0]-p0[0])*(p3[0]-p2[0])+(p1[1]-p0[1])*(p3[1]-p2[1]);
}

//-------------------------------

int Carre(int x){
  return x*x;}

//------------------------------

int NormeAuCarre(int p1[2],int p2[2]){

  //Calcul la norme au carre de p1p2

  return Carre(p1[1]-p2[1])+Carre(p1[0]-p2[0]);
}

//------------------------------

int CentreCercleCirconscrit(int A[2], int B[2],int C[2], int O[3]){

  //Stocke dans O les coordonnees du centre du cercle circonscrit a A, B et C
  //Renvoie 1 si ces 3 points sont alignes, 0 sinon et fait le calcul dans ce cas

  if(det(A,B,A,C)==0){return 1;}
   int xa=A[0]; int ya=A[1]; 
   int xb=B[0]; int yb=B[1];
   int xc=C[0]; int yc=C[1];
   O[0]=(int)((float)((yc-yb)*(xb*xb+yb*yb-xa*xa-ya*ya)-(yb-ya)*(xc*xc+yc*yc-xb*xb-yb*yb))/(float)(2*det(A,B,B,C)));
   O[1]=(int)((float)((xc-xb)*(xb*xb+yb*yb-xa*xa-ya*ya)-(xb-xa)*(xc*xc+yc*yc-xb*xb-yb*yb))/(float)(2*det(B,C,A,B)));
   return 0;
}

//-------------------------------

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

//--------------------------------------

void AffichageTriangulation(bool delaunay, int n, int sommet[][2], int t, triangle T[]){

  //Affichage des n points du plan dont les coordonnees sont dans sommet[][2] et d'une triangulation
  //en t triangles stockes dans T, tableau de taille t.
  //Produit en sortie un fichier Triangulation.ps si delaunay=false  ou un fichier Delaunay.ps
  //si delaunay=true

   ofstream output;
   if(delaunay){output.open("Delaunay.ps");}
   else{output.open("TriangIncrementale.ps");}
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

//----------------------------------------

void AffichageTestCercleCirconscrit(int sommet[4][2]){

  //Affiche les 4 points de sommet et le cercle circonscrit au trois premiers
  //Pour tester la fonction StrictementDansLeCercleCirconscrit

   ofstream output;
   output.open("Test.ps");//
   output << "%!PS-Adobe-3.0" << endl;
   output << endl;  
   for(int i=0;i<4;i++){
     output << sommet[i][0] << " " << sommet[i][1] << " 2 0 360 arc" <<endl;
     output << "0 setgray" <<endl;
     output << "fill" <<endl;
     output << sommet[i][0]+2 << " " << sommet[i][1] << " moveto" <<endl;
     output << "/Courier findfont 15 scalefont setfont" << endl;
     output << "("<< i+1 << ")" << " show" << endl;
     output << "stroke" << endl;
     output << endl;
   }
   output << endl;
   int O[2];  //Centre du cercle circonscrit
   if(CentreCercleCirconscrit(sommet[0],sommet[1],sommet[2],O)==1){cout << "Points alignes" << endl; exit(1);}
     output << O[0] << " " << O[1] << " 2 0 360 arc" <<endl;
     output << "0 setgray" <<endl;
     output << "fill" <<endl;
     output << O[0]+2 << " " << O[1] << " moveto" <<endl;
     output << "/Courier findfont 15 scalefont setfont" << endl;
     output << "(O)" << " show" << endl;
     output << "stroke" << endl;
     output << endl;

     int r; //Rayon du cercle circonscrit
     r=(int)sqrt((double)NormeAuCarre(sommet[0],O));
     output << O[0] << " " << O[1] << " " << r << " 0 360 arc" <<endl;
     output << "0 setgray" <<endl;
     output << "stroke" << endl;
     output << endl;
}


/*******************************************************************/
//Reperer un point par rapport au cercle circonscrit a un triangle
/*******************************************************************/

bool MemeCote(int A[2], int B[2], int C[2], int D[2]){

  //Renvoie vrai si C et D sont du meme cote de (AB).

  //
  // A COMPLETER
  //

}

//-----------------------------------------

bool StrictementDansLeCercleCirconscritMemeCote(int A[2], int B[2], int C[2], int D[2]){

  //Renvoie vrai si D est strictement dans le cercle circonscrit de ABC.
  //On suppose C et D du meme cote de (AB).

  //
  // A COMPLETER
  //


}

//----------------------------------------

bool StrictementDansLeCercleCirconscrit(int A[2], int B[2], int C[2], int D[2]){

  //Renvoie vrai si D est strictement dans le cercle circonscrit de ABC, quelquesoit sa position.

  //
  // A COMPLETER
  //
}


/************************************************************************/
//Calcul d'une premiere triangulation (algo incremental par ex), vous pouvez reprendre votre code...
/************************************************************************/


void TriLexicographique(int n, int sommet[][2], int t, int Tri[]){
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

//-----------------------------------------

int TriangulIncrementale(int n, int sommet[][2], int tri[], triangle T[]){
  int envconv[n];
  int taille_envconv=3;
  envconv[0]=tri[2];
  if(EstADroite(sommet[tri[2]],sommet[tri[0]],sommet[tri[1]])){
    envconv[1]=tri[1];
    envconv[2]=tri[0];}
  else{
    envconv[1]=tri[0];
    envconv[2]=tri[1];}
  int nbre_triangle=1;
  T[0].a=tri[0]; T[0].b=tri[1]; T[0].c=tri[2];

  for(int i=3;i<n;i++){//insertion de tri[i]
    int k=0;
    while((EstADroite(sommet[tri[i]],sommet[envconv[k]],sommet[envconv[k+1]]))&&(k<taille_envconv-1)){
      T[nbre_triangle].a=tri[i];
      T[nbre_triangle].b=envconv[k];
      T[nbre_triangle].c=envconv[k+1];
      nbre_triangle++;
      k++;
    }
    int khaut=k;
    k=0;
    if(EstADroite(sommet[tri[i]],sommet[envconv[taille_envconv-1]],
		  sommet[envconv[0]])){
      T[nbre_triangle].a=tri[i];
      T[nbre_triangle].b=envconv[taille_envconv-1];
      T[nbre_triangle].c=envconv[0];
      nbre_triangle++;
      k=taille_envconv-1;
      while(EstADroite(sommet[tri[i]],sommet[envconv[k-1]],
		  sommet[envconv[k]])){
      T[nbre_triangle].a=tri[i];
      T[nbre_triangle].b=envconv[k-1];
      T[nbre_triangle].c=envconv[k];
      nbre_triangle++;
      k--;
      }
    }
    int kbas=k;
    
    int envconv_tmp[taille_envconv+1];// Mise a jour d'envconv, copie temporaire

    //a faire plus proprement: int insertion=2 while(k!=kbas){k++, insertion++, ...

    if(kbas==0){kbas=taille_envconv; envconv[kbas]=envconv[0];}
    for(int j=0;j<kbas-khaut+1;j++){
      envconv_tmp[j+1]=envconv[khaut+j];}
    envconv_tmp[0]=tri[i];
    
    taille_envconv=kbas-khaut+2;
    for(int j=0;j<taille_envconv;j++){
      envconv[j]=envconv_tmp[j];}

  }
  return nbre_triangle;
}

/*******************************************************************/
//Triangulation de Delaunay
/*******************************************************************/

bool TrianglesAdjacents(triangle s, triangle t){

  //renvoie vrai si s et t ont une arete en commun

  int sommet_commun=0;
  if((s.a==t.a)||(s.a==t.b)||(s.a==t.c)){sommet_commun++;}
  if((s.b==t.a)||(s.b==t.b)||(s.b==t.c)){sommet_commun++;}
  if((s.c==t.a)||(s.c==t.b)||(s.c==t.c)){sommet_commun++;}
  if(sommet_commun>=2){return true;}
  return false;
}

//--------------------------------------------

void TrianglesVoisins(int t, triangle T[], int voisin[][3]){
 
  //Calcul pour chaque triangle les indices des triangles qui lui sont adjacents

  for(int i=0;i<t;i++){
    for(int j=0;j<i;j++){
      if(TrianglesAdjacents(T[i],T[j])){
	int case_libre;
	if(voisin[i][0]==-1){case_libre=0;}
	   else{if(voisin[i][1]==-1){case_libre=1;}
	   else{case_libre=2;}}
	voisin[i][case_libre]=j;
	if(voisin[j][0]==-1){case_libre=0;}
	else{if(voisin[j][1]==-1){case_libre=1;}
	else{case_libre=2;}}
	voisin[j][case_libre]=i;
      }
    }
  }
}
     
//--------------------------------------------

int TroisiemePoint(triangle s, triangle t){

  //Renvoie l'indice du point de s qui n'est pas dans t

  if((s.a!=t.a)&&(s.a!=t.b)&&(s.a!=t.c)){return s.a;}
  if((s.b!=t.a)&&(s.b!=t.b)&&(s.b!=t.c)){return s.b;}
  if((s.c!=t.a)&&(s.c!=t.b)&&(s.c!=t.c)){return s.c;}
  return -1;
}

//----------------------------------------------

void Flip(int i, int j, int t, triangle T[], int voisin[][3]){

  //Effectue un 'flip' entre les triangles T[i] et T[j]
  //t est la taille du tableau T et voisin, tableau calcule
  //par la fonction TrianglesVoisins, sera mis-a-jour

  int vieux_voisins_Ti[3]={voisin[i][0],voisin[i][1],voisin[i][2]};
  int vieux_voisins_Tj[3]={voisin[j][0],voisin[j][1],voisin[j][2]};
  int x=TroisiemePoint(T[i],T[j]);// On nomme les 4 sommets
  int y=TroisiemePoint(T[j],T[i]);
  int z;
  int w;
  if(T[i].a==x){z=T[i].b; w=T[i].c;}
  if(T[i].b==x){z=T[i].a; w=T[i].c;}
  if(T[i].c==x){z=T[i].a; w=T[i].b;}
  T[i].a=z;T[i].b=x;T[i].c=y;  // mise a jour de Ti et Tj
  T[j].a=w;T[j].b=x;T[j].c=y;
  //mise a jour des voisins: A faire plus proprement...
 for(int i=0;i<t;i++){
    voisin[i][0]=-1;
    voisin[i][1]=-1;
    voisin[i][2]=-1;
  }
  TrianglesVoisins(t,T,voisin);
}

//---------------------------------------------

void Delaunay(int n, int sommet[][2],int t, triangle T[]){

  //Retourne dans T un tableau de t triangles formant la triangulation de
  //Delaunay de sommet[n][2], au moment de l'appel, T contient les triangles
  //d'une triangulation quelconque de sommet[][2]


  int voisin[t][3]; // Creation et initialisation de voisin, tableau contenant pour chaque triangle l'indice dans T des triangles
                    // qui lui sont adjacents, il y en a au maximum 3, si il y en a moins la ligne correspondante est completee avec 
                    // la valeur -1
  for(int i=0;i<t;i++){
    voisin[i][0]=-1;
    voisin[i][1]=-1;
    voisin[i][2]=-1;
  }
  TrianglesVoisins(t,T,voisin);

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
  triangle T[2*n];
  for(int i=0;i<n;i++){tri[i]=i;}
  PointAuHasard(n,sommet);
  AffichagePoints(n,sommet);
  TriLexicographique(n,sommet,n,tri);
  int t=TriangulIncrementale(n,sommet,tri,T);
  AffichageTriangulation(false,n,sommet,t,T);
 
  //Lignes a decommenter, une fois l'implementation realisee

  //Delaunay(n,sommet,t,T);
  //AffichageTriangulation(true,n,sommet,t,T);
}

