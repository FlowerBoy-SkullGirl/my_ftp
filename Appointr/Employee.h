#include <iostream>
#include <string>
#include "Person.h"
#include "Appointment.h"
#include "Schedule.h"
#include <vector>

#ifndef EMPLOYEE_H
#define EMPLOYEE_H

class Employee: public Person{

	protected:
		String employee_id;
		Schedule schedule;
	public:
		String get_employee_id();

		//Mutators
		void set_employee_id(String emp_id);
		void set_employee_id(int emp_id);
		
}
#endif
