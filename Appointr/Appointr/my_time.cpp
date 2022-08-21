#include <iostream>
#include <string>
#include "my_time.h"

My_Time::My_Time()
{
    set_hour(0);
    set_minute(0);
}

My_Time::My_Time(int h, int m)
{
    set_hour(h);
    set_minute(m);
}

int My_Time::get_hour()
{
    return hour;
}

int My_Time::get_minute()
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

bool My_Time::operator==(My_Time &time)
{
    if (hour != time.hour)
        return false;
    if (minute != time.minute)
        return false;
    return true;
}

bool My_Time::operator!=(My_Time &time)
{
    return !((*this) == time);
}
