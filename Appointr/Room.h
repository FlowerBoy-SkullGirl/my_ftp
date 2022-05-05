#include <iostream>
#include <string>
#include "Appointment.h"

#ifndef ROOM_H
#define ROOM_H

class Room{
	private:
		int room_num;
		vector<Appointment> schedule;
	public:
		//Construct
		Room(int room);
		
		//Access
		int get_room_number();
		vector<Appointment> &get_appointments();

		//mutators
		void set_room_number(int num);
		void add_appt(Appointment &newAppt);
		void rem_appt(Appointment &apptToRem);
}
#endif
