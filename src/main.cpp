#include "ui/MainWindow.hpp"

#include <QApplication>

int
main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("LCM");

    MainWindow w;

    w.show();
    return a.exec();
}
