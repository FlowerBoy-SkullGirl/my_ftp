#include <iostream>
#include <string>
#include <vector>
#include "My_Date.cpp"
#include "My_Time.cpp"
#include "Appointment.cpp"
#include "Workday.cpp"
#include "Schedule.cpp"
#include "Client.cpp"
#include "Employee.cpp"

using namespace std;

int main()
{
	Workday calendar[32]; //Days of month so subscript can ignore 0 and look legible
	

	Appointment appt;
	Client client;
	Employee employee;
	My_Date date;
	My_Time daytime;
	
	employee.set_id(33);
	client.set_id(4);

	date.set_mday(12);
	date.set_wday(3);
	date.set_month(5);
	date.set_year(2022);

	daytime.set_hour(11);
	daytime.set_minute(30);
		
	My_Time opening_hours;
	opening_hours.set_hour(8);
	opening_hours.set_minute(0);
	
	My_Time closing_hours;
	closing_hours.set_hour(16);
	closing_hours.set_minute(0);

	calendar[12].set_is_open(true);
	calendar[12].set_opening(opening_hours);
	calendar[12].set_closing(closing_hours);

	appt.set_date(date);
	appt.set_timeday(daytime);
	appt.set_room(3);
	appt.set_duration(60);
	appt.set_employee(employee.get_id());
	appt.set_client(client.get_id());

	client.schedule.add_appt(appt);
	employee.schedule.add_appt(appt);

	calendar[12].add_appt(appt);


	return 0;
}

