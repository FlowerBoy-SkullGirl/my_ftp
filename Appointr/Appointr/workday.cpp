#include <iostream>
#include <vector>
#include "workday.h"

void Workday::add_appt(Appointment appt)
{
    if (!conflicts(appt))
        appt_list.push_back(appt);
}

void Workday::set_opening(My_Time open)
{
    day_begin = open;
}

void Workday::set_close(My_Time closing)
{
    day_end = closing;
}

void Workday::set_is_open(bool open)
{
    is_open = open;
}

const std::vector<Appointment> Workday::get_list() const
{
	return appt_list;
}

bool Workday::conflicts(Appointment appt)
{
    for (auto iter = appt_list.begin(); iter != appt_list.end(); iter++){
        if (*iter == appt){
            return true;
        }else if ((*iter).time_overlaps(appt)){
            return true;
        }
    }
    return false;
}

const int Workday::num_data() const
{
	int num_column = 0;
	for (auto iter = appt_list.begin(); iter != appt_list.end(); iter++){
		num_column++;
	}
	return num_column;
}

void Workday::set_date(My_Date _date)
{
    date = _date;
}

std::string Workday::str_out()
{
	std::string temp;
	My_Time ttime;
	

	for (auto iter = appt_list.begin(); iter != appt_list.end(); iter++){
		ttime = (*iter).get_timeday();
		temp += std::to_string(ttime.get_hour());
		temp += ":";
		if ( ttime.get_minute() < 10)
			temp+='0';
		temp += std::to_string(ttime.get_minute());
		temp += ' ';
		temp += "Employee number: ";
		temp += std::to_string((*iter).get_employee());
		temp += ' ';
		temp += std::to_string((*iter).get_duration());
		temp += " minutes long\n";
	}

	return temp;
}
