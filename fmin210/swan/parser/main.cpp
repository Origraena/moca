#include <iostream>
#include <fstream>

#include <stdlib.h>
#include <string.h>

using namespace std;

int main(int argc, char** argv) {

	const char* filename = argv[1];
	
	ifstream filestream(filename,ios::in);
	string line = "";

	double timestampInit = -1;
	double timestampEnd = -1;
	double tse;

	int cplus[4];
	int cmoins[4];
	int cd[4];
	int cr[4];
	long plus[4];
	long moins[4];
	long d[4];
	long r[4];
	for (int i = 0 ; i < 4 ; i++) {
		plus[i] = 0;
		moins[i] = 0;
		d[i] = 0;
		r[i] = 0;
		cplus[i] = 0;
		cmoins[i] = 0;
		cd[i] = 0;
		cr[i] = 0;

	}

	char type;
	int flowID;
	int taille;
		filestream >> line;
	while (strstr(line.c_str(),"END") == 0) {
		type = line[0];
		filestream >> line;	// tps
		if (timestampInit < 0)
			timestampInit = atof(line.c_str());
		tse = atof(line.c_str());
		if (timestampEnd < tse)
			timestampEnd = tse;
		filestream >> line; // orig
		filestream >> line; // dest
		filestream >> line; // type pkt
		filestream >> line; // taille
		taille = atoi(line.c_str());
		filestream >> line; // ----
		filestream >> line; // flow id
		flowID = line[0] - 49;
		switch (type) {
			case '+' :
				plus[flowID]+=taille;
				cplus[flowID]++;
				break;
			case '-' :
				moins[flowID]+=taille;
				cmoins[flowID]++;
				break;
			case 'd' :
				d[flowID]+=taille;
				cd[flowID]++;
				break;
			case 'r' :
				r[flowID]+=taille;
				cr[flowID]++;
				break;
			default :
				break;
		}
	
		filestream >> line;
		filestream >> line;
		filestream >> line;
		filestream >> line;
		filestream >> line;
	}

	std::cout << "Duree : " << timestampEnd - timestampInit << std::endl;
	for (int i = 0 ; i < 4 ; i++) {
		std::cout << "lien " << i << std::endl;
		std::cout << "\tnb packets :" << std::endl;
		std::cout << "\tplus = " << cplus[i] << " ; moins = " << cmoins[i] << " ; d = " << cd[i] << " ; r = " << cr[i] << std::endl; 
		std::cout << "\ttaille (octets) : " << std::endl;
		std::cout << "\tplus = " << plus[i] << " ; moins = " << moins[i] << " ; d = " << d[i] << " ; r = " << r[i] << std::endl; 
		std::cout << "\ttaille (kbits) : " << std::endl;
		std::cout << "\tplus = " << plus[i]*8/1024 << " ; moins = " << moins[i]*8/1024 << " ; d = " << d[i]*8/1024 << " ; r = " << r[i]*8/1024 << std::endl; 
		cout << "\tDebit moyen : " << moins[i] / (timestampEnd - timestampInit) << " octets/s" << endl;
		cout << "\tDebit moyen : " << (moins[i]*8) / ((timestampEnd - timestampInit) * 1024) << " kbits/s" << endl;
		cout << "\tTaux perte : " << ((double)(cplus[i]-cmoins[i]) *100) / (double)cplus[i] << '%' << endl;
		cout << "\tTaux perte octets : " << ((double)(plus[i]-moins[i]) *100) / (double)plus[i] << '%' << endl;
		cout << endl;
	}

	filestream.close();
	return 1;
}

