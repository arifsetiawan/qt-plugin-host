#include "uploadsettingsdialog.h"
#include "uploadinterface.h"

#include <QDebug>

UploadSettingsDialog::UploadSettingsDialog(QWidget *parent) :
    QDialog(parent)
{
    ui.setupUi(this);

    settings = new QSettings("pluginhost.ini", QSettings::IniFormat, this);

    ui.lineQueueSize->setText(settings->value("upload/queueSize",2).toString());
    ui.lineChunkSize->setText(settings->value("upload/chunkSize",500*1024).toString());
    ui.lineUploadUrl->setText(settings->value("upload/uploadUrl",QUrl("http://localhost:3002/files")).toString());
    ui.linePatchVerb->setText(settings->value("upload/patchVerb","PATCH").toString());
    ui.lineUserAgent->setText(settings->value("upload/userAgent","UploadPlugin/1.0.0").toString());

    int protocol = static_cast<UploadInterface::UploadProtocol>(settings->value("upload/protocol",UploadInterface::ProtocolTus).toInt());
    ui.radioButtonTus->setChecked(protocol == 1);
    ui.radioButtonMultipart->setChecked(protocol == 0);

    ui.radioButtonTus->setFocus();
}

void UploadSettingsDialog::saveSettings()
{
    settings->setValue("upload/queueSize", ui.lineQueueSize->text());
    settings->setValue("upload/chunkSize", ui.lineChunkSize->text());
    settings->setValue("upload/uploadUrl", ui.lineUploadUrl->text());
    settings->setValue("upload/patchVerb", ui.linePatchVerb->text());
    settings->setValue("upload/userAgent", ui.lineUserAgent->text());
    settings->setValue("upload/protocol", ui.radioButtonTus->isChecked() ? 1 : 0);
}
