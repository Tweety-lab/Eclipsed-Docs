#ifndef DIALOGUESYNTAX_H
#define DIALOGUESYNTAX_H

#include <QSyntaxHighlighter>

class DialogueSyntax : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    explicit DialogueSyntax(QTextDocument *parent = nullptr);

protected:
    void highlightBlock(const QString &text) override;
    void highlightLine(const QString &text, const QString &pattern, const QTextCharFormat &format);
};

#endif // DIALOGUESYNTAX_H
