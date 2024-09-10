#pragma once
#include <QtWidgets/qapplication.h>
#include <QtCore>
#include "client.h"

class App : public QApplication
{
    Q_OBJECT
public:
    App(int &argc, char **argv);
    bool sendData(Client &aClient);

signals:
    void disconnect();

private:
    void process(const QByteArray &aData);

private:
    QString mFileName;
};
