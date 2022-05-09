#include <iostream>
#include <vector>
#include "My_Date.h"
#include "My_Time.h"
#include "Appointment.h"
#include "Workday.h"

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

bool Workday::conflicts(Appointment appt)
{
	for (auto iter = appt_list.begin(); iter != appt_list.end(); iter++){
		if (*iter == appt){
			return true;
		}else if (*iter.time_overlaps(appt)){
			return true;
		}
	}
	return false;
}
