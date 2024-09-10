#include "client.h"
#include <iostream>

static inline QByteArray IntToArray(qint32 source);

Client::Client(QObject* parent) : QObject(parent)
{
    mSocket = std::make_unique<QTcpSocket>(new QTcpSocket(this));
}

bool Client::connectToHost(const QString &aHost, const quint16& aPort)
{
    std::cout << "\nConnect to the server " << aHost.toStdString() << "...";
    mSocket->connectToHost(aHost, aPort);
    return mSocket->waitForConnected();
}

bool Client::sendData(const QByteArray& aData)
{
    std::cout << "\nSend text data...";
    if (mSocket->state() == QAbstractSocket::ConnectedState)
    {
        mSocket->write(IntToArray(aData.size()));
        auto writeResult = mSocket->write(aData);
        if (writeResult == -1)
            std::cout << "\nError occured: Data hasn't been sent.";
        else if (mSocket->waitForBytesWritten())
        {
            std::cout << "\n" << writeResult << " bytes has been sent.";
            return waitForAnswer();
        }
    }
    else
        std::cout << "\nError occured: Socket has no connected state.";
    return false;
}

bool Client::waitForAnswer()
{
    std::cout << "\nWaiting for answer from the server...";
    connect(mSocket.get(), SIGNAL(disconnected()), mSocket.get(), SLOT(deleteLater()));
    bool wasAnswerObtained = mSocket->waitForReadyRead();
    if (wasAnswerObtained)
    {
        QByteArray data = mSocket->readAll();
        emit dataReceived(data);
    }
    else
        std::cout << "\nError occured: No answer from the host";
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
    std::cout << "\nDisconnect...";
    mSocket->disconnectFromHost();
}
