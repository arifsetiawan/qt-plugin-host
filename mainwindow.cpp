#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtGui>
#include <QDebug>
#include <QDir>
#include <QHeaderView>
#include <QPluginLoader>
#include <QDesktopServices>
#include <QItemDelegate>
#include <QTreeWidgetItem>

#include "newdialog.h"
#include "uploadsettingsdialog.h"
#include "downloadsettingsdialog.h"
#include "downloaditemviewdelegate.h"
#include "uploaditemviewdelegate.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setupUi();

    settings = new QSettings("pluginhost.ini", QSettings::IniFormat, this);

    loadPlugins();
}

MainWindow::~MainWindow()
{
    delete ui;
}

DownloadItem MainWindow::downloadForRow(int row) const
{
    return downloads.at(row);
}

UploadItem MainWindow::uploadForRow(int row) const
{
    return uploads.at(row);
}

void MainWindow::on_newdownloadButton_clicked()
{
    NewDialog *newDownloadDialog = new NewDialog;
    newDownloadDialog->setWindowTitle("Add new download");
    newDownloadDialog->deleteLater();
    if (!newDownloadDialog->exec())
        return;

    DownloadItem job;
    job.url = newDownloadDialog->resourcePath();
    job.progress = 0;
    downloads << job;

    QTreeWidgetItem *item = new QTreeWidgetItem(ui->downloadTreeWidget);

    item->setText(0, job.url);
    item->setText(1, "");
    item->setText(2, "0");
    item->setText(3, "0.0 KB/s");
    item->setText(4, tr("Idle"));

    downloader->append(job.url);
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
    ui->downloadTreeWidget->clear();
    downloads.clear();
}

void MainWindow::on_settingsDownloadButton_clicked()
{
    DownloadSettingsDialog *dialog = new DownloadSettingsDialog;
    dialog->setWindowTitle("Download Settings");
    dialog->deleteLater();
    if (!dialog->exec())
        return;

    dialog->saveSettings();

    // assume to update on OK event
    downloader->setFilePath(settings->value("download/filePath", QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation)).toString());
    downloader->setExistPolicy(static_cast<DownloadInterface::AlreadyExistPolicy>(settings->value("download/existPolicy",DownloadInterface::ExistThenOverwrite).toInt()));
    downloader->setPartialPolicy(static_cast<DownloadInterface::PartialExistPolicy>(settings->value("download/partialPolicy",DownloadInterface::PartialThenContinue).toInt()));
    downloader->setQueueSize(settings->value("download/queueSize",2).toInt());
    downloader->setUserAgent(settings->value("download/userAgent","DownloadPlugin/1.0.0").toByteArray());
}

void MainWindow::on_newUploadButton_clicked()
{
    NewDialog *newUploadDialog = new NewDialog;
    newUploadDialog->setWindowTitle("Add new upload");
    newUploadDialog->deleteLater();
    if (!newUploadDialog->exec())
        return;

    UploadItem job;
    job.path = newUploadDialog->resourcePath();
    job.progress = 0;
    uploads << job;

    QTreeWidgetItem *item = new QTreeWidgetItem(ui->uploadTreeWidget);

    item->setText(0, job.path);
    item->setText(1, "");
    item->setText(2, "0");
    item->setText(3, "0.0 KB/s");
    item->setText(4, tr("Idle"));

    uploader->append(job.path);
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
    ui->uploadTreeWidget->clear();
    uploads.clear();
}

void MainWindow::on_settingsUploadButton_clicked()
{
    UploadSettingsDialog *dialog = new UploadSettingsDialog;
    dialog->setWindowTitle("Upload Settings");
    dialog->deleteLater();
    if (!dialog->exec())
        return;

    dialog->saveSettings();

    // assume to update on OK event
    uploader->setQueueSize(settings->value("upload/queueSize",2).toInt());
    uploader->setChunkSize(settings->value("upload/chunkSize",500*1024).toInt());
    uploader->setUploadUrl(settings->value("upload/uploadUrl",QUrl("http://localhost:8080/files")).toUrl());
    uploader->setUploadProtocol(static_cast<UploadInterface::UploadProtocol>(settings->value("upload/protocol",UploadInterface::ProtocolTus).toInt()));
    uploader->setPatchVerb(settings->value("upload/patchVerb","PATCH").toByteArray());
    uploader->setUserAgent(settings->value("upload/userAgent","UploadPlugin/1.0.0").toByteArray());
}

void MainWindow::loadPlugins()
{
    QDir pluginsDir(QCoreApplication::applicationDirPath());
    qDebug() << "Plugin path" << pluginsDir.path();

    QStringList filters;
    filters << "*.dll";
    foreach (QString fileName, pluginsDir.entryList(filters, QDir::Files)) {
        QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName));
        QObject *plugin = pluginLoader.instance();
        if (plugin) {
            DownloadInterface *d = qobject_cast<DownloadInterface *>(plugin);
            if (d) {
                downloader = d;
                qDebug() << "Plugin" << fileName << "is loaded";
                setupDownloader();
            }

            UploadInterface *u = qobject_cast<UploadInterface *>(plugin);
            if (u) {
                uploader = u;
                qDebug() << "Plugin" << fileName << "is loaded";
                setupUploader();
            }
        }
        else {
            qWarning() << pluginLoader.errorString();
        }
    }
}

void MainWindow::setupDownloader()
{
    downloader->setFilePath(settings->value("download/filePath", QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation)).toString());
    downloader->setExistPolicy(static_cast<DownloadInterface::AlreadyExistPolicy>(settings->value("download/existPolicy",DownloadInterface::ExistThenOverwrite).toInt()));
    downloader->setPartialPolicy(static_cast<DownloadInterface::PartialExistPolicy>(settings->value("download/partialPolicy",DownloadInterface::PartialThenContinue).toInt()));
    downloader->setQueueSize(settings->value("download/queueSize",2).toInt());
    downloader->setUserAgent(settings->value("download/userAgent","DownloadPlugin/1.0.0").toByteArray());

    connect(downloader, SIGNAL(filenameSet(QString,QString)),
            this, SLOT(onFilenameSet(QString,QString)));
    connect(downloader, SIGNAL(finished(QString,QString)),
            this, SLOT(onDownloadFinished(QString,QString)));
    connect(downloader, SIGNAL(progress(QString,qint64,qint64,double,double,QString)),
            this, SLOT(onDownloadProgress(QString,qint64,qint64,double,double,QString)));
    connect(downloader, SIGNAL(status(QString,QString,QString,QString)),
            this, SLOT(onDownloadStatus(QString,QString,QString,QString)));
}

void MainWindow::onFilenameSet(const QString &url, const QString &fileName)
{
    int row = rowOfDownload(url);
    QTreeWidgetItem *item = ui->downloadTreeWidget->topLevelItem(row);
    if (item) {
        downloads[row].path = fileName;
        downloads[row].progress = 0;
        item->setText(1, fileName);
    }
}

void MainWindow::onDownloadFinished(const QString &url, const QString &fileName)
{
    // play music?
}

void MainWindow::onDownloadProgress(const QString &url, const qint64 bytesReceived, const qint64 bytesTotal,
                        const double percent, const double speed, const QString &unit)
{
    int row = rowOfDownload(url);
    QTreeWidgetItem *item = ui->downloadTreeWidget->topLevelItem(row);
    if (item) {
        downloads[row].progress = percent;
        item->setText(3, QString::number(speed) + QString(" ") + QString(unit));
    }
}

void MainWindow::onDownloadStatus(const QString &url, const QString &status, const QString &message, const QString &data)
{
    int row = rowOfDownload(url);
    QTreeWidgetItem *item = ui->downloadTreeWidget->topLevelItem(row);
    if (item) {
        downloads[row].status = status;
        item->setText(4, status);
    }
}

void MainWindow::setupUploader()
{
    uploader->setQueueSize(settings->value("upload/queueSize",2).toInt());
    uploader->setChunkSize(settings->value("upload/chunkSize",500*1024).toInt());
    uploader->setUploadUrl(settings->value("upload/uploadUrl",QUrl("http://localhost:8080/files")).toUrl());
    uploader->setUploadProtocol(static_cast<UploadInterface::UploadProtocol>(settings->value("upload/protocol",UploadInterface::ProtocolTus).toInt()));
    uploader->setPatchVerb(settings->value("upload/patchVerb","PATCH").toByteArray());
    uploader->setUserAgent(settings->value("upload/userAgent","UploadPlugin/1.0.0").toByteArray());

    connect(uploader, SIGNAL(urlSet(QString,QString)),
            this, SLOT(onUrlSet(QString,QString)));
    connect(uploader, SIGNAL(finished(QString,QString)),
            this, SLOT(onUploadFinished(QString,QString)));
    connect(uploader, SIGNAL(progress(QString,qint64,qint64,double,double,QString)),
            this, SLOT(onUploadProgress(QString,qint64,qint64,double,double,QString)));
    connect(uploader, SIGNAL(status(QString,QString,QString,QString)),
            this, SLOT(onUploadStatus(QString,QString,QString,QString)));
}

int MainWindow::rowOfDownload(const QString &url) const
{
    int row = 0;
    foreach (DownloadItem d, downloads) {
        if (d.url == url)
            return row;
        ++row;
    }
    return -1;
}

int MainWindow::rowOfUpload(const QString &path) const
{
    int row = 0;
    foreach (UploadItem u, uploads) {
        if (u.path == path)
            return row;
        ++row;
    }
    return -1;
}

void MainWindow::onUrlSet(const QString &path, const QString &url)
{
    qDebug() << "onUrlSet" << path << url;
    int row = rowOfUpload(path);
    qDebug() << row;
    QTreeWidgetItem *item = ui->uploadTreeWidget->topLevelItem(row);
    if (item) {
        uploads[row].url = url;
        uploads[row].progress = 0;
        item->setText(1, url);
    }
}

void MainWindow::onUploadFinished(const QString &path, const QString &submitUrl)
{
    // play music?
}

void MainWindow::onUploadProgress(const QString &path, const qint64 bytesSent, const qint64 bytesTotal,
                                  const double percent, const double speed, const QString &unit)
{
    int row = rowOfUpload(path);
    QTreeWidgetItem *item = ui->uploadTreeWidget->topLevelItem(row);
    if (item) {
        uploads[row].progress = percent;
        item->setText(3, QString::number(speed) + QString(" ") + QString(unit));
    }
}

void MainWindow::onUploadStatus(const QString &path, const QString &status, const QString &message, const QString &data)
{
    int row = rowOfUpload(path);
    QTreeWidgetItem *item = ui->uploadTreeWidget->topLevelItem(row);
    if (item) {
        uploads[row].status = status;
        item->setText(4, status);
    }
}

void MainWindow::setupUi()
{
    // Initialize static headers string
    QStringList downloadHeaders;
    downloadHeaders << tr("Url") << tr("File") << tr("Progress")
                    << tr("Rate") << tr("Status");
    ui->downloadTreeWidget->setHeaderLabels(downloadHeaders);
    ui->downloadTreeWidget->setItemDelegate(new DownloadItemViewDelegate(this));
    ui->downloadTreeWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->downloadTreeWidget->setAlternatingRowColors(true);
    ui->downloadTreeWidget->setRootIsDecorated(false);

    QStringList uploadHeaders;
    uploadHeaders << tr("File") << tr("Url") << tr("Progress")
            << tr("Rate") << tr("Status");
    ui->uploadTreeWidget->setHeaderLabels(uploadHeaders);
    ui->uploadTreeWidget->setItemDelegate(new UploadItemViewDelegate(this));
    ui->uploadTreeWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->uploadTreeWidget->setAlternatingRowColors(true);
    ui->uploadTreeWidget->setRootIsDecorated(false);

    // Set header resize modes and initial section sizes
    QFontMetrics fm = fontMetrics();
    QHeaderView *downloadHeaderView = ui->downloadTreeWidget->header();
    downloadHeaderView->resizeSection(0, fm.width("http://qt.digia.com/Global/Images/Qt/Files/EducationMaterial/1/L9%20-%20pdf.zip"));
    downloadHeaderView->resizeSection(1, fm.width(downloadHeaders.at(1) + "http://qt.digia.com/Global/Images/Qt/Files/EducationMaterial/1/L9%20-%20pdf.zip"));
    downloadHeaderView->resizeSection(2, fm.width(downloadHeaders.at(2) + "                  "));
    downloadHeaderView->resizeSection(3, qMax(fm.width(downloadHeaders.at(3) + "  "), fm.width(" 1234.0 bytes/s ")));
    downloadHeaderView->resizeSection(4, qMax(fm.width(downloadHeaders.at(4) + "  "), fm.width(tr("Downloading") + "  ")));

    QHeaderView *uploadHeaderView = ui->uploadTreeWidget->header();
    uploadHeaderView->resizeSection(0, fm.width("http://qt.digia.com/Global/Images/Qt/Files/EducationMaterial/1/L9%20-%20pdf.zip"));
    uploadHeaderView->resizeSection(1, fm.width(uploadHeaders.at(1) + "http://qt.digia.com/Global/Images/Qt/Files/EducationMaterial/1/L9%20-%20pdf.zip"));
    uploadHeaderView->resizeSection(2, fm.width(uploadHeaders.at(2) + "                  "));
    uploadHeaderView->resizeSection(3, qMax(fm.width(uploadHeaders.at(3) + "  "), fm.width(" 1234.0 bytes/s ")));
    uploadHeaderView->resizeSection(4, qMax(fm.width(uploadHeaders.at(4) + "  "), fm.width(tr("Uploading") + "  ")));

    // Set default tab
    ui->tabWidget->setCurrentIndex(0);

    // Set about data
    ui->aboutTextEdit->setReadOnly(true);
    ui->aboutTextEdit->setText("<html><body><h1>Qt upload and download plugin host</h1></body></html>");

    // hide some buttons
    ui->startDownloadButton->setVisible(false);
    ui->pauseDownloadButton->setVisible(false);
    ui->cancelDownloadButton->setVisible(false);
    ui->startUploadButton->setVisible(false);
    ui->pauseUploadButton->setVisible(false);
    ui->cancelUploadButton->setVisible(false);
}
