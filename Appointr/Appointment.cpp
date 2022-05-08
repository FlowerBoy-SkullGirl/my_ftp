#include <iostream>
#include "Appointment.h"

const int MAX_DUR = 480;
const int MIN_ID = 1;
const int MAX_ID = 99999999;

//Constructor
Appointment::Appointment(My_Date _date, My_Time time_of_day, int roomn, int dur, int emp_id, int client)
{
	set_date(_date);
	set_timeday(time_of_day);
	set_room(roomn);
	set_duration(dur);
	set_employee(emp_id);
	set_client(client);
}

//Accessors
My_Date Appointment::get_date()
{
	return *this.date;
}

My_Time Appointment::get_timeday()
{
	return *this.time_day;
}

int Appointment::get_room()
{
	return *this.room_num;
}
}

int Appointment::get_duration()
{
	return *this.duration_min;
}

int Appointment::get_employee()
{
	return *this.employee_id;
}

int Appointment::get_client()
{
	return *this.client_id;
}

//Mutators
void Appointment::set_date(int _date)
{
	date = _date;
}

void Appointment::set_timeday(int time_of_day)
{
	time_day = time_of_day;
}

void Appointment::set_room(int roomn)
{
	if (roomn > 0)
		room_num = roomn;
	else
		room_num = 1;
}

void Appointment::set_duration(int dur)
{
	if (dur > 0 && dur < MAX_DUR)
		duration_min = dur;
	else
		duration_min = 1;
}

void Appointment::set_employee(int id)
{
	if (id >= MIN_ID)
		employee_id = id;
	else
		employee_id = MAX_ID;
}

void Appointment::set_client(int id)
{
	if (id >= MIN_ID)
		client_id = id;
	else
		client_id = MAX_ID;
}

