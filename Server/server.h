#pragma once
#include <QtCore>
#include <QtNetwork\QTcpServer>
#include <QtNetwork\QTcpsocket>

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject* parent = 0);

signals:
    void dataReceived(QTcpSocket* aSocket, const QByteArray &aData);

public slots:
    bool sendData(QTcpSocket* aSocket, const QByteArray &aData);

private slots:
    void newConnection();
    void obtainData();
    void disconnected();

private:
    std::unique_ptr<QTcpServer> mServer;
    QHash<QTcpSocket*, QByteArray*> mBuffers; //We need a buffer to store data until block has completely received
    QHash<QTcpSocket*, char*> mSizes; //We need to store the size to verify if a block has received completely
};