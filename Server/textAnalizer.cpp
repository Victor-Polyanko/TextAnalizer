#include "textAnalizer.h"
#include <qjsondocument.h>
#include <QJsonObject>

TextAnalizer::TextAnalizer()
{
}

QByteArray TextAnalizer::analize(const QByteArray& aInput)
{
    QString text = QString::fromUtf8(aInput);
    size_t numberOfWords = 0;
    bool isWord = false;
    bool isContraction = false;
    for (auto &symbol : text)
        if (isWord)
        {
            if (symbol.isLetter())
                continue;
            if (symbol == '\'')
            {
                if (isContraction)
                    isWord = false;
                isContraction = isWord;
            }
            else
                isWord = false;
        }
        else
        {
            if (symbol.isLetter())
            {
                isWord = true;
                ++numberOfWords;
            }
        }
    QJsonObject obj;
    obj["number"] = QString::number(numberOfWords);
    QJsonDocument doc(obj);
    return doc.toJson();
}