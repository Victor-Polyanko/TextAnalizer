#pragma once
#include <QtCore>
#include <QtNetwork\QTcpsocket>

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject* aParent = 0);

signals:
    void dataReceived(const QByteArray &aData);

public slots:
    bool connectToHost(const QString &aHost, const quint16 &aPort);
    bool sendData(const QByteArray &aData);
    void disconnect();

private:
    bool waitForAnswer();

private:
    std::unique_ptr<QTcpSocket> mSocket;
};
