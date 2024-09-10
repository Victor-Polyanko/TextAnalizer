#pragma once
#include <QByteArray>

class TextAnalizer
{
public:
    TextAnalizer();
    QByteArray analize(const QByteArray& aInput);
};
