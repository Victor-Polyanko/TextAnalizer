#include "client.h"
#include <iostream>

const QString HOST = "127.0.0.1";
quint16 PORT = 1024;

Client::Client(int& argc, char** argv) : QApplication(argc, argv)
, mSocket(std::make_unique<QTcpSocket>(new QTcpSocket(this)))
{
    std::cout << "Client started...";
    init(argc, argv);
}

bool Client::init(int& argc, char** argv)
{
    if (argc == 1)
    {
        std::cout << "\nFileName is not set. Exit.";
        return false;
    }
    auto fileName = QString(argv[1]);
    if (fileName.isEmpty())
    {
        std::cout << "\nError occured: Input file name hasn't been set.";
        return false;
    }
    QFile textFile = fileName;
    if (!textFile.exists())
    {
        std::cout << "\nError occured: File " << fileName.toStdString() << " doesn't exist.";
        return false;
    }
    if (!textFile.open(QIODevice::ReadOnly))
    {
        std::cout << "\nError occured: File " << fileName.toStdString() << " can't be read.";
        return false;
    }
    if (!connectToHost(HOST, PORT))
    {
        std::cout << "\nError occured: " << HOST.toStdString() << " not found.";
        return false;
    }
    mSendBuffer = textFile.readAll();
    return true;
}

bool Client::connectToHost(const QString &aHost, const quint16& aPort)
{
    std::cout << "\nConnect to the server " << aHost.toStdString() << "...";
    mSocket->connectToHost(aHost, aPort);
    connect(mSocket.get(), &QTcpSocket::readyRead, this, &Client::readSocket);
    connect(mSocket.get(), &QAbstractSocket::errorOccurred, this, &Client::displayError);
    return mSocket->waitForConnected();
}

const bool Client::isReadyForSending() const
{
    return !mSendBuffer.isEmpty();
}

bool Client::sendData()
{
    std::cout << "\nSend text data...";
    if (mSocket->isOpen())
    {
        QDataStream socketStream(mSocket.get());
        socketStream.setVersion(QDataStream::Qt_5_15);
        socketStream << mSendBuffer;
        std::cout << "\n" << mSendBuffer.size() << " bytes has been sent.";
    }
    else
    {
        std::cout << "\nError occured: Socket is not opened for sending text.";
        return false;
    }
    return true;
}

void Client::readSocket()
{
    std::cout << "\nObtain result of text analisys from the server...";
    QByteArray buffer;
    QDataStream socketStream(mSocket.get());
    socketStream.setVersion(QDataStream::Qt_5_15);
    socketStream.startTransaction();
    socketStream >> buffer;
    if (!socketStream.commitTransaction())
    {
        std::cout << "\n" << mSocket->socketDescriptor() << ": Waiting for more data to come...";
        return;
    }
    std::cout << "\nData has been obtained.";
    process(buffer);
}

void Client::process(const QByteArray& aData)
{
    std::cout << "\nAnswer from the server: ";
    QJsonDocument doc = QJsonDocument::fromJson(aData);
    QJsonObject obj = doc.object();
    auto valueOf = [&obj](const QString& aKey) {
        return (obj.contains(aKey) ? obj[aKey].toString() : "No " + aKey + " has been obtained").toStdString();
    };
    std::cout << "\n    Number of words: " << valueOf("number");
    std::cout << "\n    Number of unique words: " << valueOf("unique");
    std::cout << "\n    The length of the longest sequence of unique words: " << valueOf("uniqueLength");
    std::cout << "\n    The longest sequence of unique words: " << valueOf("uniqueSequence");
    disconnect();
}

void Client::disconnect()
{
    std::cout << "\nDisconnect and exit...";
    if (mSocket)
    {
        mSocket->deleteLater();
        mSocket = nullptr;
    }
    exit();
}

void Client::displayError(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
        std::cout << "\nThe host was not found. Please check the host name and port settings.";
        break;
    case QAbstractSocket::ConnectionRefusedError:
        std::cout << "\nThe connection was refused by the peer. Make sure QTCPServer is running, and check that the host name and port settings are correct.";
        break;
    default:
        std::cout << "\nThe following error occurred: " << mSocket->errorString().toStdString();
        break;
    }
}
