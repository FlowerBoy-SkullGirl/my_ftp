#include <iostream>
#include <string>

#ifndef MY_DATE_H
#define MY_DATE

class My_Date{
	
	private:
		int m_day;
		int w_day;
		int month;
		int year;
		
		//sub 0 will be used for february on leap years
		int[13] mday_MAX = {29,31,28,31,30,31,30,31,31,30,31,30,31}

		//Private mutator
		void set_wday();

	public:
		//Constructor
		My_Date(int mday, int wday, int mon, int _year);

		//Accessors	
		int get_mday();
		int get_wday();
		int get_month();
		int get_year();
		bool is_leap();

		//Mutators
		void set_mday(int day);
		void set_month(int mon);
		void set_year(int y);

};
#endif
