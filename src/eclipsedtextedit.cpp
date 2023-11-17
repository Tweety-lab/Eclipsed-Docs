// eclipsedtextedit.cpp
#include "eclipsedtextedit.h"
#include <QTextBlock>
#include <QPainter>

EclipsedTextEdit::EclipsedTextEdit(QWidget *parent)
    : QTextEdit(parent)
{
}

void EclipsedTextEdit::paintEvent(QPaintEvent *event)
{
    // Call the base class paintEvent to ensure proper functionality
    QTextEdit::paintEvent(event);

    // Highlight the current line
    highlightCurrentLine();
}


void EclipsedTextEdit::highlightCurrentLine()
{
    QTextCursor cursor = textCursor();
    cursor.clearSelection();

    QTextEdit::ExtraSelection selection;

    // Set the background color for the selected line
    selection.format.setBackground(QColor(38, 38, 38));

    // Get the geometry of the current block
    QTextBlock block = cursor.block();

    // Set the selection range to the entire line
    selection.cursor = cursor;
    selection.cursor.clearSelection();
    selection.cursor.setPosition(block.position()); // Move to the beginning of the block

    // Set the background rectangle to cover the entire line
    selection.format.setProperty(QTextFormat::FullWidthSelection, true);

    // Add the selection to the list of extra selections
    QList<QTextEdit::ExtraSelection> extraSelections;
    extraSelections.append(selection);

    // Apply the extra selections to the text edit
    setExtraSelections(extraSelections);
}
