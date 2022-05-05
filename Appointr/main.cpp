#include <iostream>
#include <string>
#include <vector>
#include "Appointment.h"
#include "Client.h"
#include "Employee.h"
#include "My_Date.h"
#include "My_Time.h"
#include "Room.h"

using namespace std;

struct appt_info{
	Appointment appt;
	Client client;
	Employee employee;
	Room room;
};

//Retrieve info for an already set appt
appt_info *get_appt_info();

//Write an appointment struct to database
void make_appt(appt_info appt);

//Display all scheduled appts in chron order
void display_schedule();

//Duplicate an appt
Appointment &dup_appt(Appointment &appt);

//Reschedule an appt
void resched(Appointment &appt);

//Check for schedule conflicts
bool is_conflict(vector<Appointment> main, vector<Appointment> sub);

//Retrieve all database info
void open_database();

//Write all database info
void write_database();



int main()
{
	if (!open_database()){
		cerr << "Error: Could not open database" << endl;
		exit(0);
	}

	return 0;
}
