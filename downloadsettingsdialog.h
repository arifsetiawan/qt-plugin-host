#ifndef DOWNLOADSETTINGSDIALOG_H
#define DOWNLOADSETTINGSDIALOG_H

#include <QDialog>
#include <QSettings>

#include "ui_downloadsettings.h"

class DownloadSettingsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit DownloadSettingsDialog(QWidget *parent = 0);

public slots:
    void saveSettings();

private:
    Ui_DownloadSettingsDialog ui;
    QSettings *settings;
};

#endif // DOWNLOADSETTINGSDIALOG_H
