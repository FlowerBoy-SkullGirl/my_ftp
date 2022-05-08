#include <iostream>
#include <string>
#include "Appointment.h"
#include <vector>

#ifndef SCHEDULE_H
#define SCHEDULE_H

class Schedule{
	
	private:
		vector<Appointment> schedule;
	public:
		//Access
		vector<Appointment>::iterator appt_at(int subscript);
		vector<Appointment>::iterator container_begin();
		vector<Appointment>::iterator container_end();
		bool contains(Appointment appt);
		
		//Mutate
		void add_appt(Appointment &appt);
		void rem_appt(Appointment &appt);

};		
#endif
