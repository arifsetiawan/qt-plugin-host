#include "newdialog.h"

#include <QPushButton>

NewDialog::NewDialog(QWidget *parent) :
    QDialog(parent)
{
    ui.setupUi(this);

    connect(ui.lineEdit, SIGNAL(textChanged(QString)),
            this, SLOT(setResource(QString)));

    ui.lineEdit->setFocus();

    enableOkButton();
}

QString NewDialog::resourcePath()
{
    return ui.lineEdit->text();
}

void NewDialog::setResource(const QString &resource)
{
    if (resource.isEmpty()) {
        enableOkButton();
        return;
    }

    ui.lineEdit->setText(resource);
    enableOkButton();
}

void NewDialog::enableOkButton()
{
    ui.buttonBox->button(QDialogButtonBox::Ok)->setEnabled(!ui.lineEdit->text().isEmpty());
}
