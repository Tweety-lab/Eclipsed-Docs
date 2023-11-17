#include "extensionwarningdialog.h"

ExtensionWarningDialog::ExtensionWarningDialog(QWidget *parent)
    : QDialog(parent)
{
    setupUi();
    retranslateUi();

    // Set the default resolution
    setFixedSize(400, 100);

    // Set styling of dialog
    setStyleSheet("QWidget { color: rgb(200, 200, 200); background-color: rgb(40, 40, 40); }");
}

void ExtensionWarningDialog::setupUi()
{
    gridLayout = new QGridLayout(this);
    warning = new QLabel(this);
    warning_base = new QLabel(this);
    buttonBox = new QDialogButtonBox(this);

    gridLayout->addWidget(warning, 0, 0, 1, 1);
    gridLayout->addWidget(warning_base, 1, 0, 1, 1);
    gridLayout->addWidget(buttonBox, 2, 0, 1, 1);

    warning_base->setWordWrap(true);

    buttonBox->setOrientation(Qt::Horizontal);
    buttonBox->setStandardButtons(QDialogButtonBox::Cancel | QDialogButtonBox::Ok);

    QObject::connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    QObject::connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

void ExtensionWarningDialog::retranslateUi()
{
    setWindowTitle(tr("Extension Warning!"));
    warning->setText(tr("WARNING!"));
    warning_base->setText(tr("The file you are attempting to open is NOT an Eclipsed Complete Dialogue file (.ECD). This can cause formatting issues and not all features are supported for generic text files."));
}
