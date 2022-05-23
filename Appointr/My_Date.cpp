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

//If month is 0 for leap february, return 2 for february
int My_Date::get_month()
{
	if (month == 0)
		return 2;
	else
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

//If month is a leap february, use leap parameters, else check regular parameters
void My_Date::set_mday(int day)
{
	int mon = month;
	if (is_leap() && (mon == 2 || mon ==0))
		mon = 0;
	if (day < 1 || day > mday_MAX[mon])
		m_day = 1;
	else
		m_day = day;
}

//Using the epoch as a reference, calculate the number of days since, and determine weekday from that
void My_Date::set_wday()
{
	
}

//Allow 0 as a substitute for leap februaries, default to january
void My_Date::set_month(int mon)
{
	if (mon >= 0 && mon < 13)
		month = mon;
	else
		month = 1;
}

//Be pretty liberal with the allowed years here, really could be anything
void My_Date::set_year(int y)
{
	year = y;
}
