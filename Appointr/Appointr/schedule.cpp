#include <iostream>
#include <string>
#include "appointment.h"
#include <vector>
#include "schedule.h"

//Finds the nth iterator of a std::vector, returns end() on failure
std::vector<Appointment>::iterator Schedule::appt_at(int subscript)
{
    int i = subscript;

    for (auto iter = schedule.begin(); iter != schedule.end() && i <=0; iter++){
        if (i == 0)
            return iter;
        i--;
    }
    return schedule.end();
}

//Returns iterator to appointment if it exists, otherwise returns end()
std::vector<Appointment>::iterator Schedule::find(Appointment appt)
{
    for (auto iter = schedule.begin(); iter != schedule.end(); iter++){
        if (*iter == appt)
            return iter;
    }
    return schedule.end();
}

std::vector<Appointment>::iterator Schedule::container_begin()
{
    return schedule.begin();
}

std::vector<Appointment>::iterator Schedule::container_end()
{
    return schedule.end();
}

bool Schedule::contains(Appointment appt)
{
    for (auto iter = schedule.begin(); iter != schedule.end(); iter++){
        if (*iter == appt)
            return true;
    }
    return false;
}

//returns without adding appt if the appt conflicts with anything in the schedule
void Schedule::add_appt(Appointment &appt)
{
    if (contains(appt))
        return;
    for (auto iter = schedule.begin(); iter != schedule.end(); iter++){
        if ((*iter).time_overlaps(appt))
            return;
    }
    schedule.push_back(appt);
}

void Schedule::rem_appt(Appointment &appt)
{
    if (contains(appt)){
        schedule.erase(find(appt));
    }
}
