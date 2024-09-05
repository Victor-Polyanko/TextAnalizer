#include "app.h"

int main(int argc, char **argv)
{
    Server server;
    App app(argc, argv, server);
    return app.exec();
}