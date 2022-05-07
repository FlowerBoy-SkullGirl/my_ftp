#include <iostream>
#include <string>
#include <vector>
#include "My_Date.cpp"
#include "My_Time.cpp"
#include "Appointment.cpp"
#include "Schedule.cpp"
#include "Client.cpp"
#include "Employee.cpp"

using namespace std;

int main()
{
	Appointment appt;
	Client client;
	Employee employee;
	My_Date date;
	My_Time daytime;
	
	date.set_mday(12);
	date.set_wday(3);
	date.set_month(5);
	date.set_year(2022);

	daytime.set_hour(11);
	daytime.set_minute(30);

	appt.set_date(date);
	appt.set_timeday(daytime);
	appt.set_room(3);

	client.schedule.add_appt(appt);
	employee.set_employee_id(33);
	employee.schedule.add_appt(appt);

	Schedule main_schedule;
	
	//Checks if appt time, date, and room num is already occupied
	if (!main_schedule.contains(appt))
		main.schedule.add_appt(appt);


	return 0;
}

