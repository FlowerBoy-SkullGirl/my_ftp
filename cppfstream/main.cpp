#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>

using namespace std;

	int main(){
		
		string filen = "filenamelol";
		string filestuff;
		const int buff = 600;
		fstream fp;
		fp.open(filen.c_str(), ios::out);	
		fp << filen;
		fp.close();
		fp.open(filen.c_str(), ios::in);
		getline(fp, filestuff);
		cout << filestuff;
		fp.close();

		return 0;
	}
