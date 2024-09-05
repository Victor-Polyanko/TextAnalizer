#include "app.h"
#include <iostream>

int main(int argc, char** argv)
{
    Client client;
    App app(argc, argv);
    if (!app.setClient(client))
        return -1;
    return app.exec();
}