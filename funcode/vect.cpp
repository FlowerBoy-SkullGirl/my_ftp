#include <iostream>
#include <vector>


using namespace std;


int main(){

	vector<int> v {10, 20};
	vector<int> b = {11, 21};
	
	cout << v[1] << v[0] << b[1] << b[0] << endl;

	return 0;
}
