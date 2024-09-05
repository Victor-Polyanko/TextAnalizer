#include "client.h"
#include <iostream>

static inline QByteArray IntToArray(qint32 source);

Client::Client(QObject* parent) : QObject(parent)
{
    mSocket = std::make_unique<QTcpSocket>(new QTcpSocket(this));
}

bool Client::connectToHost(const QString &aHost, const quint16& aPort)
{
    qDebug() << "ConnectToHost" << aHost << "...";
    mSocket->connectToHost(aHost, aPort);
    return mSocket->waitForConnected();
}

bool Client::sendData(const QByteArray& aData)
{
    qDebug() << "SendData...";
    if (mSocket->state() == QAbstractSocket::ConnectedState)
    {
        mSocket->write(IntToArray(aData.size()));
        auto writeResult = mSocket->write(aData);
        if (writeResult == -1)
            std::cout << "Error occured: Data hasn't been sent.";
        else if (mSocket->waitForBytesWritten())
        {
            std::cout << writeResult << " bytes has been sent.\n";
            return waitForAnswer();
        }
    }
    else
        std::cout << "Error occured: Socket has no connected state.";
    return false;
}

bool Client::waitForAnswer()
{
    qDebug() << "WaitingForAnswer...";
    connect(mSocket.get(), SIGNAL(disconnected()), mSocket.get(), SLOT(deleteLater()));
    bool wasAnswerObtained = mSocket->waitForReadyRead();
    if (wasAnswerObtained)
    {
        QByteArray data = mSocket->readAll();
        emit dataReceived(data);
    }
    else
        std::cout << "Error occured: No answer from the host";
    return wasAnswerObtained;
}

QByteArray IntToArray(qint32 source) //Use qint32 to ensure that the number have 4 bytes
{
    QByteArray temp;
    QDataStream data(&temp, QIODevice::ReadWrite);
    data << source;
    return temp;
}

void Client::disconnect()
{
    qDebug() << "Disconnect...";
    mSocket->disconnectFromHost();
}
