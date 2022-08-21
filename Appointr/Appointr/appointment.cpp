#include <iostream>
#include "appointment.h"

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
    return (*this).date;
}

My_Time Appointment::get_timeday()
{
    return (*this).time_day;
}

int Appointment::get_room()
{
    return (*this).room_num;
}

int Appointment::get_duration()
{
    return (*this).duration_min;
}

int Appointment::get_employee() const
{
    return (*this).employee_id;
}

int Appointment::get_client()
{
    return (*this).client_id;
}

//Finds overlaps in time against another appointment
bool Appointment::time_overlaps(Appointment appt)
{
    int starttime_this = this->time_in_minutes();
    int endtime_this = this->get_duration() + this->time_in_minutes();

    int starttime_appt = appt.time_in_minutes();
    int endtime_appt = appt.time_in_minutes() + appt.get_duration();

    if (endtime_appt > starttime_this && starttime_appt < endtime_this)
        return true;
    //if this ends after that starts, and this starts before that ends
    if (endtime_this > starttime_appt && starttime_this < endtime_appt)
        return true;
    return false;
}

int Appointment::time_in_minutes()
{
    return ((this->get_timeday().get_hour() * 60) + this->get_timeday().get_minute());
}

//Mutators
void Appointment::set_date(My_Date _date)
{
    date = _date;
}

void Appointment::set_timeday(My_Time time_of_day)
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

bool Appointment::operator==(Appointment &appt)
{
    if (date != appt.date)
        return false;
    if (time_day != appt.time_day)
        return false;
    if (room_num != appt.room_num)
        return false;
    if (duration_min != appt.duration_min)
        return false;
    if (employee_id != appt.employee_id)
        return false;
    if (client_id != appt.client_id)
        return false;
    return true;
}
