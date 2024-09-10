#pragma once
#include <QByteArray>
#include <QSet>

class TextAnalizer
{
public:
    TextAnalizer();
    QByteArray analize(const QByteArray& aInput);

private:
    void processText(const QString& aText);
    void moveCurrentWord(QString& aWord);

private:
    size_t mNumberOfWords;
    QSet<QString> mUniqueWords;
    QStringList mTheLongestSequence;
    QStringList mCurrentSequence;
};
