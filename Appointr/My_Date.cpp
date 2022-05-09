#include <iostream>
#include <string>
#include "My_Date.h"

//Must set year to know if leap year before month, must set month to know if day is valid before day, must set month and day to set week day accordingly
My_Date::My_Date(int mday, int mon, int _year)
{
	set_year(_year);
	set_month(mon);
	set_mday(mday);
	set_wday();
}

int My_Date::get_mday()
{
	return m_day;
}

int My_Date::get_wday()
{
	return w_day;
}

int My_Date::get_month()
{
	return month; 
}

int My_Date::get_year()
{
	return year;
}

bool My_Date::is_leap()
{
	if ((year % 4 == 0) && ((year % 100 != 0) || (year % 400 == 0))
		return true;
	return false;
}

void My_Date::set_mday(int day)
{
	int mon = month;
	if (is_leap())
		mon = 0;
	if (day < 1 || day > mday_MAX[mon])
		m_day = 1;
	else
		m_day = day;
}

void My_Date::set
