#include "downloadsettingsdialog.h"

#include <QDesktopServices>
#include "downloadinterface.h"

DownloadSettingsDialog::DownloadSettingsDialog(QWidget *parent) :
    QDialog(parent)
{
    ui.setupUi(this);

    settings = new QSettings("pluginhost.ini", QSettings::IniFormat, this);

    ui.lineDownloadFolder->setText(settings->value("download/filePath",QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation)).toString());
    ui.lineQueueSize->setText(settings->value("download/queueSize",2).toString());
    ui.lineUserAgent->setText(settings->value("download/userAgent","UploadPlugin/1.0.0").toString());

    int existPolicy = static_cast<DownloadInterface::AlreadyExistPolicy>(settings->value("download/existPolicy",DownloadInterface::ExistThenOverwrite).toInt());
    ui.radioExistCancel->setChecked(existPolicy == 0);
    ui.radioExistOverwrite->setChecked(existPolicy == 1);
    ui.radioExistRename->setChecked(existPolicy == 2);

    int partialPolicy = static_cast<DownloadInterface::PartialExistPolicy>(settings->value("download/partialPolicy",DownloadInterface::PartialThenContinue).toInt());
    ui.radioPartialContinue->setChecked(partialPolicy == 0);
    ui.radioPartialRestart->setChecked(partialPolicy == 1);

    ui.lineDownloadFolder->setFocus();
}

void DownloadSettingsDialog::saveSettings()
{
    settings->setValue("download/filePath", ui.lineDownloadFolder->text());
    settings->setValue("download/queueSize", ui.lineQueueSize->text());
    settings->setValue("download/userAgent", ui.lineUserAgent->text());
    settings->setValue("download/partialPolicy", ui.radioPartialContinue->isChecked() ? 0 : 1);
    settings->setValue("download/existPolicy", ui.radioExistCancel->isChecked() ? 0 : ui.radioExistOverwrite->isChecked() ? 1 : 2);
}
