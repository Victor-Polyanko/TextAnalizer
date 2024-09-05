#include "app.h"

App::App(int &argc, char **argv, Server& aServer) : QApplication(argc, argv)
{
    connect(&aServer, &Server::dataReceived, this, &App::process);
    connect(this, &App::sendData, &aServer, &Server::sendData);
}

void App::process(QTcpSocket* aSocket, const QByteArray& aData)
{
    QJsonObject obj;
    obj["number"] = "2";
    QJsonDocument doc(obj);
    QByteArray data = doc.toJson();
    emit sendData(aSocket, data);
}