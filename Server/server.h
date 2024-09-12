#pragma once
#include <QtCore>
#include <QtNetwork\QTcpServer>
#include <QtNetwork\QTcpsocket>
#include <QtWidgets\qapplication.h>

class Server : public QApplication
{
    Q_OBJECT
public:
    explicit Server(int& argc, char** argv);

signals:
    void dataReceived(QTcpSocket* aSocket, const QByteArray &aData);

public slots:
    bool sendData(QTcpSocket* aSocket, const QByteArray &aData);

private slots:
    void newConnection();
    void obtainData();
    void disconnect();
    void displayError(QAbstractSocket::SocketError socketError);

private:
    QTcpServer *mServer;
    QSet<QTcpSocket*> mSockets;
};
