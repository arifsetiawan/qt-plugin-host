#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QHeaderView>

#include "newdialog.h"
#include "uploadsettingsdialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Initialize static headers string
    QStringList downloadHeaders;
    downloadHeaders << tr("Url") << tr("File") << tr("Progress")
                    << tr("Rate") << tr("Status");
    ui->downloadTreeWidget->setHeaderLabels(downloadHeaders);

    QStringList uploadHeaders;
    uploadHeaders << tr("File") << tr("Url") << tr("Progress")
            << tr("Rate") << tr("Status");
    ui->uploadTreeWidget->setHeaderLabels(uploadHeaders);

    // Set header resize modes and initial section sizes
    QFontMetrics fm = fontMetrics();
    QHeaderView *downloadHeaderView = ui->downloadTreeWidget->header();
    downloadHeaderView->resizeSection(0, fm.width("http://this-is/a-typical/file-url/to-download/filename"));
    downloadHeaderView->resizeSection(1, fm.width(downloadHeaders.at(1) + "C:\\this-is\\typical\\file-path\\in-disk"));
    downloadHeaderView->resizeSection(2, fm.width(downloadHeaders.at(2) + "                  "));
    downloadHeaderView->resizeSection(3, qMax(fm.width(downloadHeaders.at(3) + "  "), fm.width(" 1234.0 KB/s ")));
    downloadHeaderView->resizeSection(4, qMax(fm.width(downloadHeaders.at(4) + "  "), fm.width(tr("Downloading") + "  ")));

    QHeaderView *uploadHeaderView = ui->uploadTreeWidget->header();
    uploadHeaderView->resizeSection(0, fm.width("C:\\this-is\\typical\\file-path\\in-disk"));
    uploadHeaderView->resizeSection(1, fm.width(uploadHeaders.at(1) + "http://this-is-a-file-url-to-upload/filename"));
    uploadHeaderView->resizeSection(2, fm.width(uploadHeaders.at(2) + "                  "));
    uploadHeaderView->resizeSection(3, qMax(fm.width(uploadHeaders.at(3) + "  "), fm.width(" 1234.0 KB/s ")));
    uploadHeaderView->resizeSection(4, qMax(fm.width(uploadHeaders.at(4) + "  "), fm.width(tr("Uploading") + "  ")));

    ui->tabWidget->setCurrentIndex(0);

    settings = new QSettings("pluginhost", QSettings::IniFormat, this);

    ui->aboutTextEdit->setReadOnly(true);
    ui->aboutTextEdit->setText("<html><body><h1>Qt upload and download plugin host</h1></body></html>");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_newdownloadButton_clicked()
{
    NewDialog *newDownloadDialog = new NewDialog;
    newDownloadDialog->setWindowTitle("Add new download");
    newDownloadDialog->deleteLater();
    if (!newDownloadDialog->exec())
        return;

    qDebug() << newDownloadDialog->resourcePath();
}

void MainWindow::on_startDownloadButton_clicked()
{

}

void MainWindow::on_pauseDownloadButton_clicked()
{

}

void MainWindow::on_cancelDownloadButton_clicked()
{

}

void MainWindow::on_clearDownloadsButton_clicked()
{

}

void MainWindow::on_settingsDownloadButton_clicked()
{

}

void MainWindow::on_newUploadButton_clicked()
{
    NewDialog *newUploadDialog = new NewDialog;
    newUploadDialog->setWindowTitle("Add new upload");
    newUploadDialog->deleteLater();
    if (!newUploadDialog->exec())
        return;

    qDebug() << newUploadDialog->resourcePath();
}

void MainWindow::on_startUploadButton_clicked()
{

}


void MainWindow::on_pauseUploadButton_clicked()
{

}

void MainWindow::on_cancelUploadButton_clicked()
{

}

void MainWindow::on_clearUploadButton_clicked()
{

}

void MainWindow::on_settingsUploadButton_clicked()
{
    UploadSettingsDialog *dialog = new UploadSettingsDialog;
    dialog->setWindowTitle("Upload Settings");
    dialog->deleteLater();
    if (!dialog->exec())
        return;
}
