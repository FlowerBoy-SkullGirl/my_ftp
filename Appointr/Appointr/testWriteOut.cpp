#include <iostream>
#include "workday.cpp"
#include "appointment.cpp"
#include "my_time.cpp"
#include "my_date.cpp"

using namespace std;

int main()
{
	My_Date date_test(1, 1, 1988);
	My_Time time_test(3, 20);
	Appointment appt_test(date_test, time_test, 1, 60, 1, 2);
	Workday test_work;
	test_work.add_appt(appt_test);

	cout << test_work.str_out();

	return 0;
}
