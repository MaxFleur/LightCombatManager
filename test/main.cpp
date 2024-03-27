#define CATCH_CONFIG_RUNNER
#include <catch2/catch.hpp>

#include <QApplication>
#include <QSettings>

int
main(int argc, char **argv)
{
#ifdef _WIN32
    _putenv("QT_QPA_PLATFORM=offscreen");
#else
    setenv("QT_QPA_PLATFORM", "offscreen", 0);
#endif

    QApplication app(argc, argv);
    app.setApplicationName("LCM");
    app.setOrganizationName("LCM");

    // CharFileHandler needs a created conf file, so create the instance now
    QSettings settings;
    settings.setValue("test_value", "value");
    settings.sync();

    return Catch::Session().run(argc, argv);
}
