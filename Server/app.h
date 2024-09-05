#pragma once
#include <QtWidgets/qapplication.h>
#include <QtCore>
#include "server.h"

class App : public QApplication
{
    Q_OBJECT
public:
    App(int &argc, char **argv, Server &aServer);

signals:
    void sendData(QTcpSocket *aSocket, const QByteArray &aData);

private:
    void process(QTcpSocket *aSocket, const QByteArray &aData);
};
