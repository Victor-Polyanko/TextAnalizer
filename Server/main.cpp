#include "app.h"

int main(int argc, char **argv)
{
    Server server(argc, argv);
    return server.exec();
}