#include "textAnalizer.h"
#include <qjsondocument.h>
#include <QJsonObject>

const char CONTRACTION = '\'';

TextAnalizer::TextAnalizer()
{
}

QByteArray TextAnalizer::analize(const QByteArray& aInput)
{
    QString text = QString::fromUtf8(aInput);
    size_t numberOfWords = 0;
    bool isContraction = false;
    QString currentWord;
    QSet<QString> uniqueWords;
    auto moveCurrentWord = [&]() {
        if (uniqueWords.contains(currentWord))
            currentWord.clear();
        else
            uniqueWords.insert(std::move(currentWord));
    };
    for (auto &symbol : text)
        if (currentWord.isEmpty())
        {
            if (symbol.isLetter())
            {
                currentWord.append(symbol);
                ++numberOfWords;
            }
        }
        else
        {
            if (symbol.isLetter())
            {
                if (isContraction)
                {
                    isContraction = false;
                    currentWord.append(CONTRACTION);
                }
                currentWord.append(symbol);
            }
            else if (symbol == CONTRACTION)
            {
                if (isContraction)
                    moveCurrentWord();
                isContraction = !isContraction;
            }
            else
                moveCurrentWord();
        }
    QJsonObject obj;
    obj["number"] = QString::number(numberOfWords);
    obj["unique"] = QString::number(uniqueWords.size());
    QJsonDocument doc(obj);
    return doc.toJson();
}