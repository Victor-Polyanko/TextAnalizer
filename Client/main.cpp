#include "client.h"

int main(int argc, char** argv)
{
    Client client(argc, argv);
    if (client.isReadyForSending())
        client.sendData();
    else
        return -1;
    return client.exec();
}