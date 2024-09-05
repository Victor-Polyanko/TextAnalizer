#include "server.h"
#include <iostream>

const QString HOST = "127.0.0.1";
const QHostAddress HOST_ADDRESS(HOST);
quint16 PORT = 1024;

static inline qint32 ArrayToInt(QByteArray source);
static inline QByteArray IntToArray(qint32 source);

Server::Server(QObject* parent) : QObject(parent)
{
    mServer = std::make_unique<QTcpServer>(new QTcpServer(this));
    connect(mServer.get(), SIGNAL(newConnection()), this, SLOT(newConnection()));
    bool listening = mServer->listen(HOST_ADDRESS, PORT);
    qDebug() << "Listening:" << (listening ? "at address " + mServer->serverAddress().toString() : " no");
}

void Server::newConnection()
{
    qDebug() << "NewConnection...";
    while (mServer->hasPendingConnections())
    {
        QTcpSocket* socket = mServer->nextPendingConnection();
        connect(socket, SIGNAL(readyRead()), SLOT(readyRead()));
        connect(socket, SIGNAL(disconnected()), SLOT(disconnected()));
        QByteArray* buffer = new QByteArray();
        char* s = new char(0);
        mBuffers.insert(socket, buffer);
        mSizes.insert(socket, s);
    }
}

void Server::disconnected()
{
    qDebug() << "Disconnected...";
    QTcpSocket* socket = static_cast<QTcpSocket*>(sender());
    QByteArray* buffer = mBuffers.value(socket);
    char* s = mSizes.value(socket);
    socket->deleteLater();
    delete buffer;
    delete s;
}

void Server::obtainData()
{
    qDebug() << "ObtainData...";
    QTcpSocket* socket = static_cast<QTcpSocket*>(sender());
    QByteArray* buffer = mBuffers.value(socket);
    char* s = mSizes.value(socket);
    qint32 size = *s;
    while (socket->bytesAvailable() > 0)
    {
        buffer->append(socket->readAll());
        while ((size == 0 && buffer->size() >= 4) || (size > 0 && buffer->size() >= size))
        {
            if (size == 0 && buffer->size() >= 4)
            {
                size = ArrayToInt(buffer->mid(0, 4));
                *s = size;
                buffer->remove(0, 4);
            }
            if (size > 0 && buffer->size() >= size)
            {
                QByteArray data = buffer->mid(0, size);
                buffer->remove(0, size);
                size = 0;
                *s = size;
                emit dataReceived(socket, data);
            }
        }
    }
}

bool Server::sendData(QTcpSocket* aSocket, const QByteArray& aData)
{
    qDebug() << "SendData...";
    if (aSocket->state() == QAbstractSocket::ConnectedState)
    {
        aSocket->write(IntToArray(aData.size()));
        aSocket->write(aData);
        return aSocket->waitForBytesWritten();
    }
    else
        return false;
}

qint32 ArrayToInt(QByteArray source)
{
    qint32 temp;
    QDataStream data(&source, QIODevice::ReadWrite);
    data >> temp;
    return temp;
}

QByteArray IntToArray(qint32 source) //Use qint32 to ensure that the number have 4 bytes
{
    QByteArray temp;
    QDataStream data(&temp, QIODevice::ReadWrite);
    data << source;
    return temp;
}
