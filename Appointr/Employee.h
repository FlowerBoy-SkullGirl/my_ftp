#include <iostream>
#include <string>
#include "Person.h"
#include "Appointment.h"
#include <vector>

#ifndef EMPLOYEE_H
#define EMPLOYEE_H

class Employee: public Person{

	protected:
		String employee_id;
		vector<Appointment> schedule;
	public:
		String get_employee_id();
		vector<Appointment> &get_appointments();

		//Mutators
		void set_employee_id(String emp_id);
		void set_employee_id(int emp_id);
		
		void add_apt(Appointment &appt);
		void rem_appt(Appointment &appt);
}
#endif
