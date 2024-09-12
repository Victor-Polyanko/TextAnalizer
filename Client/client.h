#pragma once
#include <QtCore>
#include <QtNetwork\QTcpsocket>
#include <QtWidgets/qapplication.h>

class Client : public QApplication
{
    Q_OBJECT
public:
    explicit Client(int& argc, char** argv);
    const bool isReadyForSending() const;
    bool sendData();

private:
    bool init(int& argc, char** argv);
    bool connectToHost(const QString &aHost, const quint16 &aPort);
    void readSocket();
    void process(const QByteArray& aData);
    void disconnect();
    void displayError(QAbstractSocket::SocketError socketError);

private:
    std::unique_ptr<QTcpSocket> mSocket;
    QByteArray mSendBuffer;
};
