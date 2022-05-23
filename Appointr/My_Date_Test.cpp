#include <iostream>
#include <string>
#include "My_Date.h"
#include "My_Date.cpp"

int main()
{
	My_Date date(23,3,2024);
	
	std::cout << date.get_wday() << std::endl;

	return 0;
}
