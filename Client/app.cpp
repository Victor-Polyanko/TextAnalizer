#include "app.h"
#include <iostream>
#include <QJsonObject>

const QString HOST = "127.0.0.1";
quint16 PORT = 1024;

App::App(int& argc, char** argv) : QApplication(argc, argv)
{}

bool App::setClient(Client& aClient)
{
    if (!aClient.connectToHost(HOST, PORT))
    {
        std::cout << "Error occured: " << HOST.toStdString() <<" not found.";
        return false;
    }
    connect(&aClient, &Client::dataReceived, this, &App::process);
    connect(this, &App::disconnect, &aClient, &Client::disconnect);
    QString message{"Test Message"};
    auto data = message.toUtf8();
    return aClient.sendData(data);
}

void App::process(const QByteArray &aData) 
{
    std::cout << "Answer from the server:\n";
    QJsonDocument doc = QJsonDocument::fromJson(aData);
    QJsonObject obj = doc.object();
    auto valueOf = [&obj](const QString &aKey) { 
        return (obj.contains(aKey) ? obj[aKey].toString() : "No " + aKey + " has been obtained").toStdString();
    };
    std::cout << "    Number of words: " << valueOf("number");
    std::cout << "    Number of unique words: " << valueOf("unique");
    std::cout << "    The length of the longest sequence of unique words: " << valueOf("uniqueLength");
    std::cout << "    The longest sequence of unique words: " << valueOf("uniqueSequence");
    emit disconnect();
}
