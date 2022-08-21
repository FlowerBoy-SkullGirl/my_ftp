#include <iostream>
#include <string>
#include "person.h"
#include "appointment.h"
#include "schedule.h"
#include <vector>

#ifndef EMPLOYEE_H
#define EMPLOYEE_H

class Employee: public Person{

    protected:
        Schedule schedule;
    public:

};
#endif
