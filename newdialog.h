#ifndef NEWDIALOG_H
#define NEWDIALOG_H

#include <QDialog>

#include "ui_newdialog.h"

class NewDialog : public QDialog
{
    Q_OBJECT
public:
    explicit NewDialog(QWidget *parent = 0);

    QString resourcePath();

public slots:
    void setResource(const QString &resource);

private slots:
    void enableOkButton();

private:
    Ui_ResourceDialog ui;
};

#endif // NEWDIALOG_H
