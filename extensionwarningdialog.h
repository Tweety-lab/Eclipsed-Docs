#ifndef EXTENSIONWARNINGDIALOG_H
#define EXTENSIONWARNINGDIALOG_H

#include <QDialog>
#include <QGridLayout>
#include <QLabel>
#include <QDialogButtonBox>

class ExtensionWarningDialog : public QDialog
{
    Q_OBJECT

public:
    ExtensionWarningDialog(QWidget *parent = nullptr);

private:
    QGridLayout *gridLayout;
    QLabel *warning;
    QLabel *warning_base;
    QDialogButtonBox *buttonBox;

    void setupUi();
    void retranslateUi();
};

#endif // EXTENSIONWARNINGDIALOG_H
