#ifndef SQLHIGHLIGHTER_H
#define SQLHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QTextCharFormat>

class QTextDocument;

//! Класс подсветки синтаксиса в SQL запросе

class SqlHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    //! Конструктор
    explicit SqlHighlighter(QTextDocument *parent = 0);

protected:
    //! Правила подсветки текстового блока
    void highlightBlock(const QString &text);

private:

};

#endif // SQLHIGHLIGHTER_H
