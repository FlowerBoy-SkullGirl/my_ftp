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
	public:
		//Constructor
		My_Date(int mday, int wday, int mon, int _year);

		//Accessors	
		int get_mday();
		int get_wday();
		int get_month();
		int get_year();

		//Mutators
		void set_mday(int day);
		void set_wday(int day);
		void set_month(int mon);
		void set_year(int y);

}
#endif
