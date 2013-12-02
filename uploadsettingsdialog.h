#ifndef UPLOADSETTINGSDIALOG_H
#define UPLOADSETTINGSDIALOG_H

#include <QDialog>

#include "ui_uploadsettings.h"

class UploadSettingsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit UploadSettingsDialog(QWidget *parent = 0);

signals:

public slots:

private:
    Ui_UploadSettingsDialog ui;
};

#endif // UPLOADSETTINGSDIALOG_H
