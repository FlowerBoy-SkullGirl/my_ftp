#include "save_ops.h"
#include <iostream>
#include "person.cpp"

using namespace std;

int main()
{
	vector<Person> list_of_p;
	Person temp;

	cout << temp.get_first_name() << endl;

	list_of_p = read_people_data("person.dat");

	if (!list_of_p.empty()){
		for (auto iter = list_of_p.begin(); iter != list_of_p.end(); iter++){
			cout << "Iteration count" << endl;
			if (iter == list_of_p.end())
				cout << "Is end" << endl;
			cout << (*iter).get_first_name() << endl;
		}
	}

	return 0;
}
