#include "mainwindow.h"
#include "qcalendarwidget.h"
#include "qobjectdefs.h"
#include <QCalendarWidget>
#include <QDateTimeEdit>

#include <QApplication>
#include <QTableView>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    QCalendarWidget calendarWidget;
    QDateTimeEdit dateTimeEdit;
    w.show();
    QObject::connect(&calendarWidget, SIGNAL(QCalendarWidget::clicked(QDate)), &dateTimeEdit, SLOT(QDateTimeEdit::setDate(QDate)));
    return a.exec();
}
