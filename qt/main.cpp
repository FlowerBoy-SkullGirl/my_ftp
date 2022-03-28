#include <iostream>
#include <QApplication>
#include <QLabel>
#include <QWidget>
#include <QPushButton>


	int main(int argc, char *argv[]){

		QApplication app(argc, argv);
		//This puts text on screen. Not sure how labels work
		QWidget window;
		window.setFixedSize(400, 400);
		QLabel hello("<center> Wowie, is a qt</center>", &window);
		
		//Title of window in wm's with title bar
		hello.setWindowTitle("The best qt");
		//sets size of floating window
		//hello.resize(400, 400);
		
		QFont font("Courier");

		//A button
		QPushButton *button = new QPushButton("Press me!", &window);
		button->setToolTip("Ends the program!");
		button->setFont(font);
		button->setGeometry(20, 20, 80, 20);
		
		QObject::connect(button, SIGNAL (clicked()), QApplication::instance(), SLOT (quit()));

		//assume this works like ncurses refresh
		window.show();
		
		return app.exec();
		
	}
