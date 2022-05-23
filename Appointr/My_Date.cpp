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
	if ((year % 4 == 0) && ((year % 100 != 0) || (year % 400 == 0)))
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
//Epoch is a thursday, 0 is sunday, 7%7 equals 0, so thursday is 4
void My_Date::set_wday()
{
	int epoch_year = 1970;
	int epoch_month = 1;
	int epoch_day = 1;
	int num_leap_years = (year - (epoch_year + 2)) / 4;
	num_leap_years++;
	
	epoch_year = year - epoch_year;
	epoch_month = month - epoch_month;
	epoch_day = m_day - epoch_day;
	
	int mdays_to_add = 0;
	for (int i = 1; i <= epoch_month; i++){
		mdays_to_add += mday_MAX[i];
	}
	
	mdays_to_add += num_leap_years;
	if (is_leap() && month <= 2 && !(month == 2 && m_day == 29))
		mdays_to_add--;
	
	mdays_to_add += epoch_day;
	mdays_to_add += epoch_year * 365;
	
	w_day = ((mdays_to_add + 4) % 7);
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
