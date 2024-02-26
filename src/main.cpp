#include "ui/MainWindow.hpp"

#include "CheckBoxStyle.hpp"
#include "UtilsGeneral.hpp"

#include <QApplication>
#include <QSplashScreen>

#include <chrono>
#include <thread>

int
main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("LCM");
    app.setOrganizationName("LCM");

    app.setStyle(new CheckBoxStyle());

    const auto isSystemInDarkMode = Utils::General::isSystemInDarkMode();
    QPixmap pixmap(isSystemInDarkMode ? ":/icons/logos/splash_dark.png" : ":/icons/logos/splash_light.png");
    QSplashScreen splash(pixmap);
    splash.show();

    std::this_thread::sleep_for(std::chrono::milliseconds(2000));

    MainWindow mainWindow;
    mainWindow.show();
    splash.finish(&mainWindow);

    return app.exec();
}
