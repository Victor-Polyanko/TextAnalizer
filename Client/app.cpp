#include "app.h"
#include <iostream>
#include <QJsonObject>

const QString HOST = "127.0.0.1";
quint16 PORT = 1024;

App::App(int& argc, char** argv) : QApplication(argc, argv)
{
    if (argc > 1)
        mFileName = QString(argv[1]);
}

bool App::sendData(Client& aClient)
{
    if (mFileName.isEmpty())
    {
        std::cout << "\nError occured: Input file name hasn't been set.";
        return false;
    }
    QFile currentFile(mFileName);
    if (!currentFile.exists())
    {
        std::cout << "\nError occured: File " << mFileName.toStdString() << " doesn't exist.";
        return false;
    }
    if (!currentFile.open(QIODevice::ReadOnly))
    {
        std::cout << "\nError occured: File " << mFileName.toStdString() << " can't be read.";
        return false;
    }
    auto data = currentFile.readAll();
    if (!aClient.connectToHost(HOST, PORT))
    {
        std::cout << "\nError occured: " << HOST.toStdString() <<" not found.";
        return false;
    }
    connect(&aClient, &Client::dataReceived, this, &App::process);
    connect(this, &App::disconnect, &aClient, &Client::disconnect);
    return aClient.sendData(data);
}

void App::process(const QByteArray &aData) 
{
    std::cout << "\nAnswer from the server: ";
    QJsonDocument doc = QJsonDocument::fromJson(aData);
    QJsonObject obj = doc.object();
    auto valueOf = [&obj](const QString &aKey) { 
        return (obj.contains(aKey) ? obj[aKey].toString() : "No " + aKey + " has been obtained").toStdString();
    };
    std::cout << "\n    Number of words: " << valueOf("number");
    std::cout << "\n    Number of unique words: " << valueOf("unique");
    std::cout << "\n    The length of the longest sequence of unique words: " << valueOf("uniqueLength");
    std::cout << "\n    The longest sequence of unique words: " << valueOf("uniqueSequence");
    emit disconnect();
}
