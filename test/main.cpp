#define CATCH_CONFIG_RUNNER
#include <catch2/catch.hpp>

#include <QApplication>

int
main(int argc, char** argv )
{
    QApplication app( argc, argv );

    return Catch::Session().run( argc, argv );
}
