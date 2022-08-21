#include <iostream>
#include <string>
#include "my_time.h"
#include "my_date.h"

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
        Appointment(My_Date _date, My_Time time_of_day, int roomn, int dur, int emp_id, int client);
	Appointment(){};

        //Accessors
        My_Date get_date();
        My_Time get_timeday();
        int get_duration();
        int get_room();
        int get_employee() const;
        int get_client();
        bool time_overlaps(Appointment appt);
        int time_in_minutes();

        //Mutators
        void set_date(My_Date newDate);
        void set_timeday(My_Time newTime);
        void set_room(int roomnum);
        void set_duration(int dur);
        void set_employee(int id);
        void set_client(int id);

        //Operator
        bool operator==(Appointment &appt);
};

#endif
