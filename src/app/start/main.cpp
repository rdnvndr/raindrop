#include "extendedapplication.h"

int main(int argc, char* argv[])
{
    ExtendedApplication app(argc, argv);

    app.localize();
    app.readSettings();

    if (app.initialize())
    {
        return app.exec();
    }

    return -1;
}
