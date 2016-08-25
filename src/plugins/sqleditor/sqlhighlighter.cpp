#include "sqlhighlighter.h"
#include <QTextDocument>

namespace RTPTechGroup {
namespace SqlEditor {

SqlHighlighter::SqlHighlighter(QTextDocument *parent) :
    QSyntaxHighlighter(parent)
{

}

void SqlHighlighter::highlightBlock(const QString &text)
{
    qint32 pos = 0;

    setFormat (0, text.length(), Qt::black);

    static const QRegExp commandsRegexp ("\\b(?:select|from|where|and|case|when|then|else|distinct|all|null|"
                                         "is|like|between|not|group|by|having|order|inner|outer|right|left|alter|with|isnull|cast|create|replace|function|"
                                         "returns|language|volatile|cost|table|view|or|"
                                         "join|on|using|union|exists|in|as|intersect|except|coalesce|insert|into|update)\\b",
                                         Qt::CaseInsensitive);
    pos = 0;
    while ((pos = commandsRegexp.indexIn (text, pos)) != -1)	{
        setFormat (pos, commandsRegexp.matchedLength(), Qt::magenta);
        pos += commandsRegexp.matchedLength();
    }

    static const QRegExp aggregationsRegexp ("\\b(?:count|min|max)\\b\\s*\\([^\\)]+\\)",
                                             Qt::CaseInsensitive);
    pos = 0;
    while ((pos = aggregationsRegexp.indexIn (text, pos)) != -1)	{
        setFormat (pos, aggregationsRegexp.matchedLength(), Qt::darkGreen);
        pos += aggregationsRegexp.matchedLength();
    }

    static const QRegExp numbersRegexp ("[^\\w]((\\d+)(\\.)?)",
                                        Qt::CaseInsensitive);
    pos = 0;
    while ((pos = numbersRegexp.indexIn (text, pos)) != -1)	{
        setFormat (pos, numbersRegexp.matchedLength(), Qt::blue);
        pos += numbersRegexp.matchedLength();
    }

    static const QRegExp stringsRegexp ("'[^']+'",
                                        Qt::CaseInsensitive);
    pos = 0;
    while ((pos = stringsRegexp.indexIn (text, pos)) != -1)	{
        setFormat (pos, stringsRegexp.matchedLength(), Qt::red);
        pos += stringsRegexp.matchedLength();
    }

    pos = 0;
    static const QRegExp commentRegexp ("^\\s*(--)");
    if ((pos = commentRegexp.indexIn (text, pos)) != -1)	{
        setFormat (pos, text.length(), Qt::blue);
        return;
    }

}

}}
