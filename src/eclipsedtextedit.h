// eclipsedtextedit.h
#ifndef ECLIPSEDTEXTEDIT_H
#define ECLIPSEDTEXTEDIT_H

#include <QTextEdit>

class EclipsedTextEdit : public QTextEdit
{
    Q_OBJECT

public:
    EclipsedTextEdit(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void highlightCurrentLine();
    void paintCurrentLineText();
};

#endif // ECLIPSEDTEXTEDIT_H
