#include "workdayModel.h"
#include <vector>
#include <string>
#include "appointment.h"
#include <iostream>

WorkdayModel::WorkdayModel(QObject * parent) : QAbstractTableModel(parent)
{
}

int WorkdayModel::rowCount(const QModelIndex & /*parent*/) const
{
	return work_data.data_rows();
}

int WorkdayModel::columnCount(const QModelIndex & /*parent*/) const
{
	return work_data.num_data();
}

QVariant WorkdayModel::data(const QModelIndex &index, int role) const
{
	int row = index.row();
	int col = index.column();
	std::vector<Appointment> list_appts = work_data.get_list();
	My_Time temp_time;
	My_Date temp_date;
	int temp_id;
	int temp_dur;
	std::string temp_str;
	//Grab the requested piece of data and return string for pos
	//Row 0 is Time, 1 Date, 2 Employee ID, 3 Duration of appt
	if (role == Qt::DisplayRole){
		switch (col){
			case 0:
				temp_time = list_appts[row].get_timeday();
				temp_str += temp_time.get_hour();
				temp_str += ":";
				temp_str += temp_time.get_minute();
				break;
			case 1:
				temp_date = list_appts[row].get_date();
				temp_str += temp_date.get_mday();
				temp_str += "/";
				temp_str += temp_date.get_month();
				temp_str += "/";
				temp_str += temp_date.get_year();
				break;
			case 2:
				temp_id = list_appts[row].get_employee();
				temp_str += temp_id;
				break;
			case 3:
				temp_dur = list_appts[row].get_duration();
				temp_str += temp_dur;
				break;
			}
		return QString::fromStdString(temp_str);
	}
	return QVariant();
}

void WorkdayModel::set_data(Workday data)
{
	work_data = data;
}
