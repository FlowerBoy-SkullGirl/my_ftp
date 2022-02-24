#ifndef my_string_h
#define my_string_h

class my_string;

std::ostream &operator << (std::ostream &, my_string &);
std::istream &operator >> (std::istream &, my_string &);


class my_string{

	private:
		int string_len;
		char *str = nullptr;
		
 
	public:
		my_string(); //Default constructor
		my_string(const char *src); //constructor with char* arg
		my_string(my_string &&src); //move constructor
		my_string(const my_string &src); //copy constructor
		~my_string(); //default destructor	
		const my_string &operator=(const char *src); //overload = for char*
		const my_string &operator=(const my_string &src); //overload = for class obj
		const my_string &operator=(my_string &&src); //move operator
		my_string operator+(const char *src); //overloaded + char*
		my_string operator+(const my_string &src); //overloaded + class obj
		const my_string &operator+=(const char *src); //+= char
		const my_string &operator+=(const my_string &src);
		
		char *to_c_str(); //output string
		
		int len(); //returns length in int
		
		void out(); //output test
		
		

};

#endif
