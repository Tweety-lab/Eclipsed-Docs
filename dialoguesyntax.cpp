#include "dialoguesyntax.h"
#include <QTextCharFormat>
#include <QColor>
#include <iostream>

DialogueSyntax::DialogueSyntax(QTextDocument *document)
    : QSyntaxHighlighter(document)
{
    // Initialize any member variables or setup specifics here
}

void DialogueSyntax::highlightBlock(const QString &text)
{

    // Create a text char format for NPC dialogue
    QTextCharFormat formatNPC;
    formatNPC.setForeground(QColor(0, 150, 255));
    formatNPC.setFontItalic(false);

    // Create a text char format for Player dialogue
    QTextCharFormat formatPlayer;
    formatPlayer.setForeground(QColor(50, 205, 50));
    formatPlayer.setFontItalic(false);

    // Create a text char format for Response dialogue
    QTextCharFormat formatChoice;
    formatChoice.setForeground(QColor(255, 255, 255));
    formatChoice.setFontItalic(false);

    // Define the patterns to search for
    QString patternNPC = "&N";
    QString patternPlayer = "&P";
    QString patternChoice = "	";

    // Trigger Syntax highlighting
    highlightLine(text, patternChoice, formatChoice);
    highlightLine(text, patternNPC, formatNPC);
    highlightLine(text, patternPlayer, formatPlayer);
}

// Handle line-based Syntax highlighting
void DialogueSyntax::highlightLine(const QString &text, const QString &pattern, const QTextCharFormat &format)
{
    int index = text.indexOf(pattern);

    while (index >= 0) {
        // Apply formatting to the matched pattern
        setFormat(index, pattern.length() + 1000, format);

        // Search for the next occurrence of the pattern
        index = text.indexOf(pattern, index + pattern.length());
    }
}


