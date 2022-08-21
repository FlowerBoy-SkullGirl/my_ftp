#include <iostream>
#include <string>
#include "person.h"

using std::string;

const int MAX_ID_P = 99999999;
const int MIN_ID_P = 1;

//Constructor
Person::Person()
{
    string unown = "Unknown";
    set_first_name(unown);
    set_last_name(unown);
    set_address(unown);
    set_phone(unown);
    set_id(MAX_ID_P);
}

Person::Person(string fname, string lname, string addr, string phoneNum, int idNum)
{
    set_first_name(fname);
    set_last_name(lname);
    set_address(addr);
    set_phone(phoneNum);
    set_id(idNum);
}

//Accessors
string Person::get_first_name()
{
    return (*this).first_name;
}

string Person::get_last_name()
{
    return (*this).last_name;
}

string Person::get_address()
{
    return (*this).address;
}

string Person::get_phone()
{
    return (*this).phone;
}

int Person::get_id()
{
    return (*this).id;
}

//Mutators
void Person::set_first_name(string name)
{
    (*this).first_name = name;
}

void Person::set_last_name(string name)
{
    (*this).last_name = name;
}

void Person::set_address(string addr)
{
    (*this).address = addr;
}

void Person::set_phone(string phoneNum)
{
    (*this).phone = phoneNum;
}

void Person::set_id(int id_num)
{
    if (id_num >= MIN_ID_P)
        (*this).id = id_num;
    else
        id_num = MAX_ID_P;
}
