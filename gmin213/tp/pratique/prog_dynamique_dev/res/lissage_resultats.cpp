
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <fstream>
#include <math.h>
#include <cstdlib>
#include <vector>
#include <float.h>

using namespace std;

class WithComma: public numpunct<char> // class for decimal numbers with comma
{
protected: char do_decimal_point() const { return ','; } // override the function that gives the decimal separator
};

class WithPoint: public numpunct<char> // class for decimal numbers with comma
{
protected: char do_decimal_point() const { return '.'; } // override the function that gives the decimal separator
};

int main(int argc, char* argv[], char* env[])
{
	if(argc < 2)
	{
		cerr << "lissage_resultats [file_path]" << endl;
		return -1;
	}
	
	locale locComma(locale(), new WithComma);
	locale locPoint(locale(), new WithPoint);
	ifstream donnees(argv[1], ios::in);
	ofstream out("lissage.txt", ios::out|ios::trunc);
	vector<int> temps, nbSommets;
	int t, nbS;
	unsigned int i = 0;
	
	cout << endl << "Lecture du fichier " << argv[1] << "..." << endl;
	while(!donnees.eof())
	{
		while(donnees.good())
		{
			donnees >> nbS >> t;
			
			if(donnees.good())
			{
				nbSommets.push_back(nbS);
				temps.push_back(t);
			}
			
			i++;
		}
		if(!donnees.eof())
		{
			donnees.clear();
			char temp[256];
			donnees.getline(temp, 256);
		}
	}
	donnees.close();
	cout << "Lecture du fichier terminee (" << i << " lignes)." << endl;
	
	
	cout << "Calcul des statistiques..." << endl;
	
	double nbMin = DBL_MAX, nbMax = DBL_MIN;
	for(i = 0 ; i < nbSommets.size() ; i++)
	{
		if(nbSommets[i] < nbMin)
			nbMin = nbSommets[i];
		if(nbSommets[i] > nbMax)
			nbMax = nbSommets[i];
	}
	cout << nbMin << " <= nbSommets <= " << nbMax << " ; " << nbSommets.size() << " valeurs" << endl;
	
	cout << "Valeurs lissees : " << endl;
	vector<int> nbSommetsLisse;
	vector<double> tempsLisse;
	vector<int> nbValeurs;
	int arrondi = nbMin;
	for(i = 0 ; arrondi <= nbMax ; i++)
	{
		nbSommetsLisse.push_back(arrondi);
		tempsLisse.push_back(0.0);
		nbValeurs.push_back(0);
		for(unsigned int j = 0 ; j < nbSommets.size() ; j++)
		{
			if(nbSommets[j] == arrondi)
			{
				tempsLisse[i] += temps[j];
				nbValeurs[i]++;
			}
		}
		if(nbValeurs[i] == 0)
		{
			tempsLisse[i] = -1;
		}
		else
		{
			tempsLisse[i] /= (double)(nbValeurs[i]);
		}
		
		/*cout << nbValeurs[i] << " : " << densiteLisse[i] << " " << floodLisse[i] << " " << rbopLisse[i] << " " << lbopLisse[i] << " " << bipLisse[i] << " " << lbipLisse[i] << " " << dlbipLisse[i] << endl;*/
		arrondi++;
	}
	cout << "Suppression des valeurs incorrectes" << endl;
	for(i = 0 ; i < nbSommetsLisse.size() ; i++)
	{
		if(tempsLisse[i] == -1)
		{
			nbSommetsLisse.erase(nbSommetsLisse.begin()+i);
			tempsLisse.erase(tempsLisse.begin()+i);
			nbValeurs.erase(nbValeurs.begin()+i);
			i--;
		}
	}
	cout << "Nb val\tnbSommets\ttemps" << endl;
	for(i = 0 ; i < nbSommetsLisse.size() ; i++)
	{
		cout << nbValeurs[i] << "\t" << nbSommetsLisse[i] << "\t\t" << tempsLisse[i] << endl;
		out << nbSommetsLisse[i] << " " << tempsLisse[i] << endl;
	}
	/*for(unsigned int i = 0 ; i < densite.size() ; i++)
	{
		//cout << "ligne " << i << endl;
		cout << densite[i] << " " << flood[i] << " " << rbop[i] << " " << lbop[i] << " " << bip[i] << " " << lbip[i] << " " << dlbip[i] << endl;
	}*/
	out.close();
	
	return 0;
}


