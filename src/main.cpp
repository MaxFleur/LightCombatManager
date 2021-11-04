#include "GUI/MainWindow.hpp"

#include <QApplication>

int
main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	a.setApplicationName("LCM");
	a.setOrganizationName("LCM");

	MainWindow w;

	w.show();
	return a.exec();
}
