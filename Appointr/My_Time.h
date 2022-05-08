#include <iostream>
#include <string>

#ifndef MY_TIME_H
#define MY_TIME_H

class My_Time{
	
	private:
		int hour;
		int minute;
	public:
		//Constrcutor
		My_Time(int h, int m);
		
		//Accessor
		int get_hour();
		int get_minute();
		
		//Mutator
		void set_hour(int h);
		void set_minute(int m);
};

#endif
