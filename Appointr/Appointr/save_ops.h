#include <fstream>
#include <string>
#include <iomanip>
#include <vector>
#include <sstream>
#include "person.h"
#include "employee.h"
#include "my_date.h"
#include "my_time.h"
#include "appointment.h"
#include "schedule.h"
#include "workday.h"
//Serializing will not work because STRINGS. Dynamic ARRGGH. Redo this, but with character separated values.

//Pass an object to be saved and the file it belongs to
template <class T>
void save_obj_data(T obj, std::string filen){
	using namespace std;

	fstream fp;
	fp.open(filen.c_str(), ios::out | ios::app | ios::binary);

	if (!fp.is_open()){
		cerr << "Bad file operation" << endl;
		return;
	}

	fp.write(reinterpret_cast<char *>(&obj), sizeof(obj));
	
	fp.close();
}

/*
 *Provide a temp obj(mostly so we can inform the function of the type) and a filen.
 *Returns a vector of object type to be used by program
 */

template <class T>
std::vector<T> read_obj_data(T temp, std::string filen)
{
	using namespace std;

	vector<T> list_of_obj;

	T temp2;

	fstream fp;
	fp.open(filen.c_str(), ios::in | ios::binary);
	
	if (!fp.is_open()){
		cerr << "Bad file open" << endl;
		return list_of_obj;
	}

	while (!fp.eof()){
		fp.read(reinterpret_cast<char *>(&temp2), sizeof(temp2));
		list_of_obj.push_back(temp2);
	}
	
	fp.close();

	return list_of_obj;
}

void save_people_data(Person saveme, std::string filen)
{
	const int NUM_DATA = 5;
	using namespace std;

	fstream fp;
	fp.open(filen.c_str(), ios::out | ios::app);

	if (!fp.is_open()){
		cerr << "Bad file open" << endl;
		exit(0);
	}

	fp << saveme.get_first_name() << "\\" << saveme.get_last_name() << "\\" << saveme.get_address() << "\\" << saveme.get_phone() << "\\" << saveme.get_id() << endl;

	fp.close();
}

std::vector<Person> read_people_data(std::string filen)
{
	const int NUM_DATA = 5;
	using namespace std;

	vector<Person> list_of_people;

	Person Johnathan_Data_Hands;
	string getter;
	string string_temp;
	stringstream strstream;
	vector<string> string_arr;
	int temp_int = 0;

	fstream fp;
	fp.open(filen.c_str(), ios::in);

	if (!fp.is_open()){
		cerr << "Bad file open" << endl;
		exit(0);
	}

	int i = 0;
	while (!fp.eof()){
		getline(fp, getter);
		//first arg needs stream
		strstream << getter;
		while (i < NUM_DATA && getline(strstream, string_temp, '\\')){
			string_arr.push_back(string_temp);
			i++;
		}
		
		if (i == NUM_DATA){
			Johnathan_Data_Hands.set_first_name(string_arr[0]);
			Johnathan_Data_Hands.set_last_name(string_arr[1]);
			Johnathan_Data_Hands.set_address(string_arr[2]);
			Johnathan_Data_Hands.set_phone(string_arr[3]);
			Johnathan_Data_Hands.set_id(stoi(string_arr[4]));

			list_of_people.push_back(Johnathan_Data_Hands);
		}
		i = 0;
		string_arr.clear();
		strstream.str(string());
		strstream.clear();
	}

	return list_of_people;
}
void save_workday_data(Workday save_the_day)
{
	using namespace std;
	string filen;
	vector<Appointment> list_to_save = save_the_day.get_list();
	My_Date save_date = save_the_day.get_date();
	filen += "Workday";
	filen += to_string(save_date.get_mday());
	filen += to_string(save_date.get_month());
	filen += to_string(save_date.get_year());

	fstream fp;
	fp.open(filen.c_str(), ios::in);
	
	if (fp.is_open())
		fp.close();
	
	for (auto iter = list_to_save.begin(); iter != list_to_save.end(); iter++){
		save_obj_data((*iter), filen);
	}

}
Workday read_workday_data(My_Date date)
{
	using namespace std;
	Workday read_the_day;
	read_the_day.set_date(date);
	string filen;
	filen += "Workday";
	filen += to_string(date.get_mday());
	filen += to_string(date.get_month());
	filen += to_string(date.get_year());

	Appointment fakie;
	vector<Appointment> data_read = read_obj_data(fakie, filen);

	for (auto iter = data_read.begin(); iter != data_read.end(); iter++){
		read_the_day.add_appt(*iter);
	}

	return read_the_day;
}


