// mainwindow.h

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include "dialoguesyntax.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    Ui::MainWindow* getUi() const;

private slots:
    void on_actionOpen_triggered();
    void on_actionSave_triggered();

    void on_actionSave_As_triggered();

    void on_actionAlign_Right_triggered();

    void on_actionAlign_Middle_triggered();

    void on_actionAlign_Left_triggered();

    void on_actionBold_triggered();

    void on_actionItalic_triggered();

    void on_mainTextField_textChanged();

    void on_actionNew_triggered();

    void on_actionGithub_triggered();

    void on_actionZoom_In_triggered();

    void on_actionZoom_Out_triggered();

    void on_actionMark_Player_Dialogue_triggered();

    void on_actionMark_NPC_Dialogue_triggered();

private:
    Ui::MainWindow *ui;
    DialogueSyntax *syntaxHighlighter;

};

#endif // MAINWINDOW_H
