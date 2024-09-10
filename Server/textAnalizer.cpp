#include "textAnalizer.h"
#include <iostream>
#include <QJsonDocument>
#include <QJsonObject>

const char CONTRACTION = '\'';

TextAnalizer::TextAnalizer()
    : mNumberOfWords(0)
{
}

QByteArray TextAnalizer::analize(const QByteArray& aInput)
{
    std::cout << "\nConvert data from client to text...";
    QString text = QString::fromUtf8(aInput);
    std::cout << "\nAnalize text...";
    processText(text);
    auto uniqueSequence = mTheLongestSequence.join(' ');
    std::cout << "\nAnalize completed. Text contains " << mNumberOfWords << " words (" << mUniqueWords.size() << " unique words among them).";
    std::cout << "\nThe longest sequence of different words has length " << mTheLongestSequence.size() << ": " << uniqueSequence.toStdString();
    QJsonObject obj;
    obj["number"] = QString::number(mNumberOfWords);
    obj["unique"] = QString::number(mUniqueWords.size());
    obj["uniqueLength"] = QString::number(mTheLongestSequence.size());
    obj["uniqueSequence"] = uniqueSequence;
    QJsonDocument doc(obj);
    std::cout << "\nConvert Json data to byteArray for the client...";
    return doc.toJson();
}

void TextAnalizer::processText(const QString& aText)
{
    mNumberOfWords = 0;
    mUniqueWords.clear();
    mTheLongestSequence.clear();
    mCurrentSequence.clear();
    bool isContraction = false;
    QString currentWord;
    for (const auto& symbol : aText)
        if (currentWord.isEmpty())
        {
            if (symbol.isLetter())
            {
                currentWord.append(symbol);
                ++mNumberOfWords;
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
                    moveCurrentWord(currentWord);
                isContraction = !isContraction;
            }
            else
                moveCurrentWord(currentWord);
        }
    if (!currentWord.isEmpty())
        moveCurrentWord(currentWord);
    if (mTheLongestSequence.size() < mCurrentSequence.size())
        mTheLongestSequence = std::move(mCurrentSequence);
}

void TextAnalizer::moveCurrentWord(QString &aWord)
{
    if (mUniqueWords.contains(aWord))
    {
        int wordsToDelete = mCurrentSequence.size();
        for (auto checkWord = mCurrentSequence.rbegin(); checkWord != mCurrentSequence.rend(); ++checkWord)
            if (aWord == *checkWord)
                break;
            else
                --wordsToDelete;
        if (wordsToDelete > 0)
        {
            if (mTheLongestSequence.size() < mCurrentSequence.size())
                mTheLongestSequence = mCurrentSequence;
            mCurrentSequence.remove(0, wordsToDelete);
        }
    }
    else
        mUniqueWords.insert(aWord);
    mCurrentSequence.append(std::move(aWord));
};