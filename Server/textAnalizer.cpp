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
    QStringList theLongestSequence;
    QStringList currentSequence;
    auto moveCurrentWord = [&]() {
        if (uniqueWords.contains(currentWord))
        {
            int wordsToDelete = currentSequence.size();
            for (auto checkWord = currentSequence.rbegin(); checkWord != currentSequence.rend(); ++checkWord)
                if (currentWord == *checkWord)
                    break;
                else
                    --wordsToDelete;
            if (wordsToDelete > 0)
            {
                if (theLongestSequence.size() < currentSequence.size())
                    theLongestSequence = currentSequence;
                currentSequence.remove(0, wordsToDelete);
            }
        }
        else
            uniqueWords.insert(currentWord);
        currentSequence.append(std::move(currentWord));
    };
    for (const auto &symbol : text)
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
    if (!currentWord.isEmpty())
        moveCurrentWord();
    if (theLongestSequence.size() < currentSequence.size())
        theLongestSequence = std::move(currentSequence);
    QJsonObject obj;
    obj["number"] = QString::number(numberOfWords);
    obj["unique"] = QString::number(uniqueWords.size());
    obj["uniqueLength"] = QString::number(theLongestSequence.size());
    obj["uniqueSequence"] = theLongestSequence.join(' ');
    QJsonDocument doc(obj);
    return doc.toJson();
}