#include "app.h"
#include "textAnalizer.h"

App::App(int &argc, char **argv, Server& aServer) : QApplication(argc, argv)
{
    connect(&aServer, &Server::dataReceived, this, &App::process);
    connect(this, &App::sendData, &aServer, &Server::sendData);
}

void App::process(QTcpSocket* aSocket, const QByteArray& aData)
{
    std::unique_ptr<TextAnalizer> textAnalizer(new TextAnalizer());
    auto result = textAnalizer->analize(aData);
    emit sendData(aSocket, result);
}