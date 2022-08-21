#include "person.cpp"
#include "save_ops.h"

using namespace std;

int main()
{

	Person temp;
	temp.set_first_name("Johnny");
	temp.set_last_name("Test");
	temp.set_id(12);
	save_people_data(temp, "person.dat");

	return 0;
}
