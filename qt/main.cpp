#include <iostream>
#include <QApplication>
#include <QLabel>
#include <QWidget>


	int main(int argc, char *argv[]){

		QApplication app(argc, argv);
		//This puts text on screen. Not sure how labels work
		QLabel hello("<center> Wowie, is a qt</center>");
		
		//Title of window in wm's with title bar
		hello.setWindowTitle("The best qt");
		//sets size of floating window
		hello.resize(400, 400);
		//assume this works like ncurses refresh
		hello.show();
		
		return app.exec();
		
	}
