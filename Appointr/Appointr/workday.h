#include <iostream>
#include <vector>
#include <string>
#include "my_date.h"
#include "my_time.h"
#include "appointment.h"

#ifndef WORKDAY_H
#define WORKDAY_H

class Workday{

    protected:
        std::vector<Appointment> appt_list;
        My_Time day_begin;
        My_Time day_end;
        bool is_open;
        My_Date date;
    public:
        void add_appt(Appointment appt);
        void set_opening(My_Time open);
        void set_close(My_Time closing);
        void set_is_open(bool open);
        void set_date(My_Date _date);

	const int num_data() const;
	const int data_rows() const {return 4;}
	const std::vector<Appointment> get_list() const;
	const My_Date get_date() const{return date;}

        bool conflicts(Appointment appt);

	std::string str_out();
	

};
#endif
