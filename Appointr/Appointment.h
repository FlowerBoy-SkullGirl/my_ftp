#include <iostream>
#include <string>
#include "My_Time.h"
#include "My_Date.h"
#include "Employee.h"
#include "Client.h"

#ifndef APPOINTMENT_H
#define APPOINTMENT_H

class Appointment{
	
	private:
		My_Date date;
		My_Time time_day;
		int room_num;
		int duration_min;
		int employee_id;
		int client_id;
		
	public:
		//Constructor
		Appointment(My_Date _date, My_Time time_of_day, int roomn, int dur, int emp_id, int client)
		
		//Accessors
		My_Date get_date();
		My_Time get_timeday();
		
		//Mutators
		void set_date(My_Date newDate);
		void set_timeday(My_Time newTime);
		void set_room(int roomnum);
};

#endif
