#include <iostream>
#include <string>
#include "appointment.h"
#include <vector>

#ifndef SCHEDULE_H
#define SCHEDULE_H

class Schedule{

    private:
        std::vector<Appointment> schedule;
    public:
        //Access
        std::vector<Appointment>::iterator appt_at(int subscript);
        std::vector<Appointment>::iterator find(Appointment appt);
        std::vector<Appointment>::iterator container_begin();
        std::vector<Appointment>::iterator container_end();
        bool contains(Appointment appt);

        //Mutate
        void add_appt(Appointment &appt);
        void rem_appt(Appointment &appt);

};
#endif
