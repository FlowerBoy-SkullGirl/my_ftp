#include <iostream>
#include <istream>
#include <ostream>
#include "my_string.h"
#include "my_string.cpp"

using namespace std;
	
	void test();

	int main(){
		
		my_string Hello_World;
		Hello_World = "Hello";
		
		my_string OddSpace = "Testing spaces";
		
		OddSpace = Hello_World + Hello_World; 

		cout << Hello_World << Hello_World.len() << endl;

		Hello_World += " World!";

		cout << Hello_World << endl;

		my_string Goodbye_World = Hello_World;
		
		cout << Goodbye_World << endl;
			
		test();		

		Hello_World.out();
		cout << Goodbye_World.to_c_str() << endl;
		
		cout << Hello_World << Goodbye_World.len() << endl;


		
		OddSpace = Hello_World + Hello_World;
		cout << OddSpace << endl;
	
		

		return 0;
	}


	void test(){
		cout << "testing const and dest" << endl;
		
		my_string Destroy_World;

	}
