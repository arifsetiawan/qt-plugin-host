#ifndef UPLOADSETTINGSDIALOG_H
#define UPLOADSETTINGSDIALOG_H

#include <QDialog>
#include <QSettings>

#include "ui_uploadsettings.h"

class UploadSettingsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit UploadSettingsDialog(QWidget *parent = 0);

public slots:
    void saveSettings();

private:
    Ui_UploadSettingsDialog ui;
    QSettings *settings;
};

#endif // UPLOADSETTINGSDIALOG_H
