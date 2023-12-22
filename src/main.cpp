#include "ui/MainWindow.hpp"

#include "UtilsGeneral.hpp"

#include <QApplication>

int
main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("LCM");
    app.setOrganizationName("LCM");

    MainWindow mainWindow;
    const auto isSystemInDarkMode = Utils::General::isColorDark(mainWindow.palette().color(QPalette::Window));
    app.setWindowIcon(isSystemInDarkMode ? QIcon(":/icons/logo_main_white.png") : QIcon(":/icons/logo_main_black.png"));

    mainWindow.show();
    return app.exec();
}
