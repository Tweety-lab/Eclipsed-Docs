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
    delete syntaxHighlighter;  // Don't forget to delete the allocated memory!
    delete ui;
}



QString currentFilePath; // Global variable, should see if theres a better way to handle file interaction

// If player pressed F5 button, open help link
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    // Define help link
    QUrl helpUrl("https://github.com/Tweety-lab/Eclipsed-Docs");
    if (event->key() == Qt::Key_F5)
    {
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
    else
    {
        // Call the base class implementation for other key events
        QMainWindow::keyPressEvent(event);
    }
}

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

// Make UI accessable through non MainWindow derivative functions
Ui::MainWindow* MainWindow::getUi() const {
    return ui;
}

// Define functions for later use on actionSave trigger

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


bool isItalic = false; // Define the italic variable for toggling italic text
bool isBold = false; // Define the bold variable for toggling bold text

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

// On user changed text (right now used for displaying if file has or has not been saved)

/* The entire save-display system is incredibly messy and should probably be
   Cleaned up or rewritten completely at a later date but it works for now.

   PS: This method of checking for a change in text means it will add
   a star to the WindowTitle when a new file is opened even if nothing
   is changed. Not an issue right now but may be at a later date.*/

void MainWindow::on_mainTextField_textChanged()
{
    int index = MainWindow::windowTitle().indexOf('*'); // Search for if there is a '*' present in the window title

    // If there is no '*', add one
    if (index == -1)
    {
        MainWindow::setWindowTitle(MainWindow::windowTitle() + "*");
    }
}



