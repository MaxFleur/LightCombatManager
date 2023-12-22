#include "ui/MainWindow.hpp"

#include "UtilsGeneral.hpp"

#include <QApplication>
#include <QSplashScreen>
#include <QWidget>

#include <chrono>
#include <thread>

int
main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("LCM");
    app.setOrganizationName("LCM");

    // Tiny placeholder widget to determine system color
    QWidget widget;
    const auto isSystemInDarkMode = Utils::General::isColorDark(widget.palette().color(QPalette::Window));
    QPixmap pixmap(isSystemInDarkMode ? ":/icons/logo_splash_dark.png" : ":/icons/logo_splash_light.png");
    QSplashScreen splash(pixmap);
    splash.show();

    std::this_thread::sleep_for(std::chrono::milliseconds(2000));

    MainWindow mainWindow;
    mainWindow.show();
    splash.finish(&mainWindow);

    return app.exec();
}
