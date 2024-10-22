#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "dialoguesyntax.h"
#include "extensionwarningdialog.h"

#include <QFileDialog>
#include <QTextEdit>
#include <QTextStream>
#include <QFile>
#include <QTextBlock>
#include <QDesktopServices>
#include <QUrl>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Initialize the syntaxHighlighter after ui setup
    syntaxHighlighter = new DialogueSyntax(ui->mainTextField->document());
}

MainWindow::~MainWindow()
{
    delete syntaxHighlighter;
    delete ui;
}

// GLOBAL VARIABLES:

QString currentFilePath;
bool isItalic = false; // Define the italic variable for toggling italic text
bool isBold = false; // Define the bold variable for toggling bold text


// STANDALONE FUNCTIONS:

// Insert a string at the start of the current line
void insertAtLineStart(MainWindow& mainWindow, QString val)
{
    QTextEdit* mainTextField = mainWindow.getUi()->mainTextField;
    QTextCursor textCursor = mainTextField->textCursor();

    QString currentLine = textCursor.block().text(); // Define the current line the user's cursor is on

    int count = currentLine.count("	");

    // If there is at least 1 indent on current line, place val at end of indents otherwise place val at start of line
    if (count >= 1) {
        int index = currentLine.indexOf("	"); // Get position of indent

        // Set textCursor to end of indent and insert val
        textCursor.setPosition(textCursor.block().position() + index);
        textCursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, count);
        textCursor.insertText(val);

    } else {
        // Move textCursor to start of line and insert val
        textCursor.movePosition(QTextCursor::StartOfLine);
        textCursor.insertText(val);
    }

}

// Simulate a Zoom effect by scaling the font by an inputted value
void adjustZoom(MainWindow& mainWindow, int val)
{
    QTextEdit* mainTextField = mainWindow.getUi()->mainTextField;

    // Increase the font size of all text by val
    QTextCursor cursor = mainTextField->textCursor();
    cursor.select(QTextCursor::Document);
    QTextCharFormat format;
    format.setFontPointSize(cursor.charFormat().font().pointSize() + val);
    cursor.mergeCharFormat(format);
}


// Save to pre-existing file
void saveDefault(MainWindow& mainWindow)
{
    QTextEdit* mainTextField = mainWindow.getUi()->mainTextField; // Use getUi() to access ui
    QFile file(currentFilePath);

    // Open the file in write-only mode
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        // Write all text to file
        out << mainTextField->toHtml();

        // Close the file
        file.close();
    } else {
        qDebug() << "Error opening file:" << file.errorString();
    }
    mainWindow.setWindowTitle(mainWindow.windowTitle().remove(QChar('*'))); // Remove all '*' symbols
}

// Save to new file
void saveAs(MainWindow& mainWindow) {
    // Show user file selection dialogue for saving a new file
    QString filePath = QFileDialog::getSaveFileName(nullptr, "Save File", "", "Eclipsed Files (*.ecd);;Text Files (*.txt)");
    currentFilePath = filePath;


    // Validate if the user selected a save location, return if not
    if (filePath.isEmpty()) {
        return;
    }

    QTextEdit* mainTextField = mainWindow.getUi()->mainTextField; // Use getUi() to access ui
    QFile file(filePath);
    QFileInfo fileInfo(filePath);

    // Open the file in write-only mode
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);

        // Write all text to file
        out << mainTextField->toHtml();

        // Close the file
        file.close();
    } else {
        qDebug() << "Error opening file:" << file.errorString();
    }

    // Set WindowTitle to include name of newly saved file
    mainWindow.setWindowTitle("Eclipsed Docs");
    mainWindow.setWindowTitle(mainWindow.windowTitle() + " - " + fileInfo.baseName() + "." + fileInfo.suffix());

    mainWindow.setWindowTitle(mainWindow.windowTitle().remove(QChar('*'))); // Remove all '*' symbols
}


// FILE ACTIONS:

void MainWindow::on_actionNew_triggered()
{
    ui->mainTextField->setText(""); // TODO: Check if theres a dedicated clear text function
    currentFilePath.clear(); // Reset the currentFilePath

    MainWindow::setWindowTitle("Eclipsed Docs"); // Reset the Window Title
}

// Handles opening a file
void MainWindow::on_actionOpen_triggered()
{
    // Show user file selection dialogue and update window title to display name of file
    QString filePath = QFileDialog::getOpenFileName(nullptr, "Open File", "", "Eclipsed Files (*.ecd);;Text Files (*.txt)");
    currentFilePath = filePath;

    // Validate if the user selected a file, return if not
    if (filePath.isEmpty())
    {
        return;
    }

    QFileInfo fileInfo(filePath);

    // Check if the user opened a file with a non .ecd extension
    if (fileInfo.completeSuffix() != "ecd")
    {
        // If file suffix is NOT .ecd show a warning dialog
        ExtensionWarningDialog warningDialog;
        int result = warningDialog.exec();

        // Check the result of dialog
        if (result != QDialog::Accepted) {
            // If user clicked cancel, stop the open file code.
            return;
        }
    }

    // Set WindowTitle to include name of opened file and suffix
    MainWindow::setWindowTitle("Eclipsed Docs");
    MainWindow::setWindowTitle(MainWindow::windowTitle() + " - " + fileInfo.baseName() + "." + fileInfo.completeSuffix());

    // Reset file save state
    MainWindow::setWindowTitle(MainWindow::windowTitle().remove(QChar('*')));

    /* TODO: Figure out if "fileInfo.baseName() + fileInfo.completeSuffix()" is dumb.
     Could also probably put the window title system into a function since it is repeated
     on line 127. */

    // Initialize Variables needed to project file content to mainTextField
    QTextEdit* mainTextField = ui->mainTextField;
    QFile file(filePath);

    // Open the file in read-only mode
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);

        // Read all the text from the file
        QString fileContent = in.readAll();

        // Output the content to the mainTextField
        mainTextField->setHtml(fileContent);

        // Close the file
        file.close();
    } else {
        qDebug() << "Error opening file:" << file.errorString();
    }

}

void MainWindow::on_actionSave_triggered() {
    // Checks if user is saving to pre-existing file, if not run Save As
    if(currentFilePath.isEmpty())
    {
        saveAs(*this);
        return;
    }

    // Save to pre-existing file
    saveDefault(*this);
}

void MainWindow::on_actionSave_As_triggered()
{
    saveAs(*this);
}


// TEXT ACTIONS:

/* Alignment actions, this used to be written from scratch but was later changed to use setAlignment().
Sometimes I question my choice of hobby. */

// Align Left
void MainWindow::on_actionAlign_Left_triggered()
{
    // Define the mainTextField
    QTextEdit* mainTextField = ui->mainTextField;

    // Set alignment of current line to left
    mainTextField->setAlignment(Qt::AlignLeft);
}

// Align Middle
void MainWindow::on_actionAlign_Middle_triggered()
{
    // Define the mainTextField
    QTextEdit* mainTextField = ui->mainTextField;

    // Set alignment of current line to center
    mainTextField->setAlignment(Qt::AlignCenter);
}

// Align Right
void MainWindow::on_actionAlign_Right_triggered()
{
    // Define the mainTextField
    QTextEdit* mainTextField = ui->mainTextField;

    // Set alignment of current line to right
    mainTextField->setAlignment(Qt::AlignRight);
}


// On Bold toolbar button clicked
void MainWindow::on_actionBold_triggered()
{
    // Define the mainTextField
    QTextEdit* mainTextField = ui->mainTextField;

    // Set the format to bold text
    QTextCharFormat format;

    isBold = !isBold;

    // Toggle bold text
    if (isBold)
    {
        format.setFontWeight(QFont::Normal);
    } else {
        format.setFontWeight(QFont::Bold);
    }

    // Set text to format
    mainTextField->setCurrentCharFormat(format);
}

// On Italic toolbar button clicked
void MainWindow::on_actionItalic_triggered()
{
    // Define the mainTextField
    QTextEdit* mainTextField = ui->mainTextField;

    QTextCharFormat format;

    // Toggle the state
    isItalic = !isItalic;

    // Set the text to italic or remove italic based on the state
    format.setFontItalic(isItalic);

    // Apply the format to the selected text or the text that will be inserted
    mainTextField->setCurrentCharFormat(format);
}




// MISC ACTIONS:


// On user changed text (right now used for displaying if file has or has not been saved)

/* The entire save-display system is incredibly messy and should probably be
   Cleaned up or rewritten completely at a later date but it works for now.*/

void MainWindow::on_mainTextField_textChanged()
{
    int index = MainWindow::windowTitle().indexOf('*'); // Search for if there is a '*' present in the window title

    // If there is no '*', add one
    if (index == -1)
    {
        MainWindow::setWindowTitle(MainWindow::windowTitle() + "*");
    }

}

// Make UI accessable through non MainWindow derivative functions
Ui::MainWindow* MainWindow::getUi() const {
    return ui;
}

void MainWindow::on_actionGithub_triggered()
{
    // Define help link
    QUrl helpUrl("https://github.com/Tweety-lab/Eclipsed-Docs");

    // Check if the URL is valid
    if (helpUrl.isValid())
    {
        QDesktopServices::openUrl(helpUrl);
    }
    else
    {
        qDebug() << "Invalid URL Help URL";
    }
}


// Handle zooming in/out

// Zoom In
void MainWindow::on_actionZoom_In_triggered()
{
    adjustZoom(*this, 1);
}

// Zoom Out
void MainWindow::on_actionZoom_Out_triggered()
{
    adjustZoom(*this, -1);
}

// Automatically insert '&P' at start of current line
void MainWindow::on_actionMark_Player_Dialogue_triggered()
{
    insertAtLineStart(*this, "&P ");
}

// Automatically insert '&N' at start of current line
void MainWindow::on_actionMark_NPC_Dialogue_triggered()
{
    insertAtLineStart(*this, "&N ");
}

