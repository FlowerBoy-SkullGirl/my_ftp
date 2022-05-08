#include <iostream>
#include <string>
#include "Person.h"

const int MAX_ID = 99999999;
const int MIN_ID = 1;

//Constructor
Person::Person(String fname, String lname, String addr, String phoneNum, int idNum)
{
	set_first_name(fname);
	set_last_name(lname);
	set_address(addr);
	set_phone(phoneNum);
	set_id(idNum);
}

//Accessors
String Person::get_first_name()
{
	return *this.first_name;
}

String Person::get_last_name()
{
	return *this.last_name;
}

String Person::get_address()
{
	return *this.address;
}

String Person::get_phone()
{
	return *this.phone;
}

int Person::get_id()
{
	return *this.id;
}

//Mutators
void Person::set_first_name(String name)
{
	*this.first_name = name;
}

void Person::set_last_name(String name)
{
	*this.last_name = name;
}

void Person::set_(String addr)
{
	*this.address = addr;
}

void Person::set_(String phoneNum)
{
	*this.phone = phoneNum;
}

void Person::set_(int id_num)
{
	if (id_num >= MIN_ID)
		*this.id = id_num;
	else
		id_num = MAX_ID;
}
