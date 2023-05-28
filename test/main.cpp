#define CATCH_CONFIG_RUNNER
#include <catch2/catch.hpp>

#include <QApplication>

int
main(int argc, char **argv)
{
#ifdef _WIN32
    _putenv("QT_QPA_PLATFORM", "offscreen");
#else
    setenv("QT_QPA_PLATFORM", "offscreen", 0);
#endif

    QApplication app(argc, argv);

    return Catch::Session().run(argc, argv);
}
