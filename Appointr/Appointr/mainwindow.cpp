#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "person.cpp"
#include "my_date.cpp"
#include "my_time.cpp"
#include "appointment.cpp"
#include "schedule.cpp"
#include "workday.cpp"
#include "save_ops.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Person temp;
    std::vector<Person> emp_list = read_people_data("person.dat");
    if (!emp_list.empty()){
        for (auto iter = emp_list.begin(); iter != emp_list.end(); iter++){
            ui->comboBox->addItem(QString::fromStdString((*iter).get_first_name()));
        }
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}

//Need to add code for QPushButton to getDate and getTime from TimeEdit and DateEdit when clicked().



void MainWindow::on_pushButton_clicked()
{
    int m_day;
    int month_int;
    int year_int;
    int hr_int;
    int min_int;



    m_day = ui->dateEdit->date().day();
    month_int = ui->dateEdit->date().month();
    year_int = ui->dateEdit->date().year();

    My_Date temp_date(m_day, month_int, year_int);

    hr_int = ui->timeEdit->time().hour();
    min_int = ui->timeEdit->time().minute();

    My_Time temp_time(hr_int, min_int);

    int employee_index = ui->comboBox->currentIndex();

    Appointment test_values(temp_date, temp_time, 1, 60, 1, employee_index);

    Workday workday = read_workday_data(temp_date);
    workday.set_date(temp_date);
    workday.add_appt(test_values);

    save_workday_data(workday);

}



void MainWindow::on_calendarWidget_clicked(const QDate &date)
{
    ui->textEdit->clear();
    My_Date my_date;
    my_date.set_mday(date.day());
    my_date.set_month(date.month());
    my_date.set_year(date.year());

    Workday current_day = read_workday_data(my_date);
    std::string pText = current_day.str_out();
    QString textToAdd = QString::fromStdString(pText);
    ui->textEdit->insertPlainText(textToAdd);
}

