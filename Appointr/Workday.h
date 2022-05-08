#include <iostream>
#include <vector>
#include "My_Date.h"
#include "My_Time.h"
#include "Appointment.h"

#ifndef WORKDAY_H
#define WORKDAY_H

class Workday{

	protected:
		vector<Appointment> appt_list;
		My_Time day_begin;
		My_Time day_end;
		bool is_open;
	public:
		void add_appt(Appointment appt);
		void set_opening(My_Time open);
		void set_close(My_Time closing);
		void set_is_open(bool open);
		
		bool conflicts(Appointment appt);
		
};
#endif
