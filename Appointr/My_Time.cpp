#include <iostream>
#include <string>
#include "My_Time.h"

My_Time::My_Time(int h, int m)
{
	set_hour(h);
	set_minute(m);
}

My_Time::get_hour()
{
	return hour;
}

My_Time::get_minute()
{
	return minute;
}

void My_Time::set_hour(int h)
{
	if (h >= 0 && h <= 24)
		hour = h;
	else
		hour = 0;
}

void My_Time::set_minute(int m)
{
	if (m >= 0 && m <= 60)
		minute = m;
	else
		minute = 0;
}
