#include <iostream>
#include <string>
#include "Person.h"
#include "Appointment.h"

#ifndef CLIENT_H
#define CLIENT_H

class Client: public Person{
	
	private:
		vector<Appointment> schedule;
	public:
		//Access
		vector<Appointment> &get_schedule();
		
		//Mutate
		void add_appt(Appointment &appt);
		void rem_appt(Appointment &appt);
}	

#endif
