#include <iostream>
#include <algorithm>
#include "my_string.h"
#include <istream>
#include <ostream>

using namespace std;

const int MAX = 2056;


ostream &operator << (ostream &strm, my_string &src){	
	strm << src.to_c_str();
	
	return strm;
}

istream &operator >> (istream &strm, my_string &dest){
	char *src = new char[MAX];
	strm >> src;
	
	dest = src;
	
	delete[] src;

	return strm;
}

//Constructor default
my_string::my_string(){
	string_len = 0;
}


//Strlen
int my_cstr_len(const char *src){
	int i = 0;
	while( *(src + i) != '\0' ){
		i++;
	}

	return i;
}


	

//Constructor c_str
my_string::my_string(const char *src){
	string_len = my_cstr_len(src);

	str = new char[string_len];

	for(int i = 0; i < string_len; i++){
		*(str + i) = *(src +i);
	}
}

//Destructor
my_string::~my_string(){
	if(string_len > 0){
		delete [] str;
	}
}

//assignment operator c_str
const my_string &my_string::operator=(const char *src){
	if(string_len > 0){
		delete [] str;
	}
	string_len = my_cstr_len(src);

	str = new char[string_len];

	for(int i = 0; i < string_len; i++){
		*(str + i) = *(src +i);
	}
	return *this;
}
	
//Assignment operator str obj
const my_string &my_string::operator=(const my_string &src){
	if(this != &src){
		if(string_len > 0){
			delete [] str;
		}
		string_len = src.string_len;
	
		str = new char[string_len];
		
		for(int i = 0; i < string_len; i++){
			*(str + i) = *(src.str + i);
		}
	}
	return *this;
}

//Move constructor
my_string::my_string(my_string &&src){
	string_len = src.string_len;
	str = src.str;
	
	src.str = nullptr;
	src.string_len = 0;
}

//Move operator
const my_string &my_string::operator=(my_string &&src){
	if(this != &src){
		swap(string_len, src.string_len);
		swap(str, src.str);
	}
	return *this;
}

//Copy constructor 
my_string::my_string(const my_string &src){
	string_len = src.string_len;
	str = new char[string_len];
	
	for(int i = 0; i < string_len; i++){
		*(str + i) = *(src.str +i);
	}
}

my_string my_string::operator+(const char *src){
	my_string temp;


	temp.string_len += string_len;
	temp.string_len += my_cstr_len(src);
	
	temp.str = new char[temp.string_len];
	
	int y = string_len;

	for(int i = 0; i < string_len; i++){
		*(temp.str + i) = *(str + i);
	}

	int j = 0;

	for(int i = y; i < temp.string_len; i++){
		*(temp.str +i) = *(src + j);
		j++;
	}

	return temp;
}
		
		
my_string my_string::operator+(const my_string &src){
	my_string temp;
	
	temp.string_len += src.string_len;
	temp.string_len += string_len;
	
	cout << temp.string_len << endl;

	temp.str = new char[temp.string_len];
	
	int y = string_len;
	
	for(int i = 0; i < string_len; i++){
		*(temp.str + i) = *(str + i);
	}

	int j = 0;

	for(int i = y; i < temp.string_len; i++){
		*(temp.str + i) = *(src.str + j);
		j++;
	}

	return temp;
}


char *my_string::to_c_str(){
	char *dest = new char[string_len + 1];

	for(int i = 0; i < string_len; i++){
		*(dest + i) = *(str + i);
	}
	
	*(dest + string_len + 1) = '\0';

	return dest;
}

int my_string::len(){

	return string_len;
}

const my_string &my_string::operator+=(const char *src){
	*this = *this + src;
	return *this;
}

const my_string &my_string::operator+=(const my_string &src){
	*this = *this + src;
	return *this;
} 

void my_string::out(){
	char *output = this->to_c_str();
	int i = 0;
	while( *(output + i) != '\0'){
		cout << *(output + i);
		i++;
	}
	cout << endl;
}
