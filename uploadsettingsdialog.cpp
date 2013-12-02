#include "uploadsettingsdialog.h"

UploadSettingsDialog::UploadSettingsDialog(QWidget *parent) :
    QDialog(parent)
{
    ui.setupUi(this);

    ui.radioButtonTus->setFocus();
}
