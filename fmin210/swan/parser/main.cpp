#include <iostream>
#include <fstream>

#include <stdlib.h>

using namespace std;

int main(int argc, char** argv) {

	const char* filename = argv[1];
	
	ifstream filestream(filename,ios::in);
	string line = "";

	int plus[4];
	int moins[4];
	int d[4];
	int r[4];
	for (int i = 0 ; i < 4 ; i++) {
		plus[i] = 0;
		moins[i] = 0;
		d[i] = 0;
		r[i] = 0;
	}

	char type;
	int flowID;

	while (line[0] != EOF) {
		filestream >> line;
		type = line[0];
		filestream >> line;	// tps
		filestream >> line; // orig
		filestream >> line; // dest
		filestream >> line; // type pkt
		filestream >> line; // taille
		filestream >> line; // ----
		filestream >> line; // flow id
		flowID = atoi(line.c_str()) - 1;
		filestream >> line; //
		switch (type) {
			case '+' :
				plus[flowID]++;
				break;
			case '-' :
				moins[flowID]++;
				break;
			case 'd' :
				d[flowID]++;
				break;
			case 'r' :
				r[flowID]++;
				break;
			default :
				break;
		}
		while (line != "\n")
			filestream >> line;
	}

	return 1;
}

