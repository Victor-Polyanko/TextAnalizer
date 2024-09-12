#include "server.h"
#include "textAnalizer.h"
#include <iostream>
#include <chrono>

quint16 PORT = 1024;

Server::Server(int& argc, char** argv) : QApplication(argc, argv)
{
    mServer = new QTcpServer(); 
    if (mServer->listen(QHostAddress::Any, PORT))
    {
        connect(mServer, &QTcpServer::newConnection, this, &Server::newConnection);
        std::cout << "\nServer is listening...";
    }
    else
    {
        std::cout << "\nError occured: Server can't listen.";
        exit();
    }
}

void Server::newConnection()
{
    std::cout << "\nEstablish new connection with client...";
    while (mServer->hasPendingConnections())
    {
        QTcpSocket* socket = mServer->nextPendingConnection();
        std::cout << "\nClient with socketId " << socket->socketDescriptor() << " has just sent a text";
        connect(socket, &QTcpSocket::readyRead, this, &Server::obtainData);
        connect(socket, &QTcpSocket::disconnected, this, &Server::disconnect);
        connect(socket, &QAbstractSocket::errorOccurred, this, &Server::displayError);
        mSockets.insert(socket);
    }
}

void Server::disconnect()
{
    std::cout << "\nDisconnect socket.";
    QTcpSocket* socket = static_cast<QTcpSocket*>(sender());
    QSet<QTcpSocket*>::iterator it = mSockets.find(socket);
    if (it != mSockets.end())
        mSockets.remove(*it);
    socket->deleteLater();
}

void Server::obtainData()
{
    QTcpSocket* socket = static_cast<QTcpSocket*>(sender());
    std::cout << "\nObtain text data from client " << socket->socketDescriptor();
    QByteArray buffer;
    QDataStream socketStream(socket);
    socketStream.setVersion(QDataStream::Qt_5_15);
    socketStream.startTransaction();
    socketStream >> buffer;
    if (!socketStream.commitTransaction())
    {
        std::cout << "\nWaiting for more data to come from " << socket->socketDescriptor();
        return;
    }
    std::unique_ptr<TextAnalizer> textAnalizer(new TextAnalizer());
    auto t0 = std::chrono::steady_clock::now();
    auto result = textAnalizer->analize(buffer);
    auto t1 = std::chrono::steady_clock::now();
    std::cout << "\nText was anaized in "<< std::chrono::duration_cast<std::chrono::milliseconds>(t1-t0).count() << " ms.";
    sendData(socket, result);
}

bool Server::sendData(QTcpSocket* aSocket, const QByteArray& aData)
{
    std::cout << "\nSend result of text analysis to client...";
    if (!aSocket)
    {
        std::cout << "\nError occured: Client is not connected.";
        return false;
    }
    if (!aSocket->isOpen())
    {
        std::cout << "\nError occured: Socket "<< aSocket->socketDescriptor() << "is not opened.";
        return false;
    }
    QDataStream socketStream(aSocket);
    socketStream.setVersion(QDataStream::Qt_5_15);
    socketStream << aData;
    std::cout << "\nData has been sent to " << aSocket->socketDescriptor();
    return true;
}

void Server::displayError(QAbstractSocket::SocketError socketError)
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
        QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
        std::cout << "\nThe following error occurred: " << socket->errorString().toStdString();
        break;
    }
}
