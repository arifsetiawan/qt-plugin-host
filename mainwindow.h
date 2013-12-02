#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>

#include "downloadinterface.h"
#include "uploadinterface.h"
#include "item.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    DownloadItem MainWindow::downloadForRow(int row) const;
    UploadItem MainWindow::uploadForRow(int row) const;

private slots:
    void on_newdownloadButton_clicked();
    void on_startDownloadButton_clicked();
    void on_pauseDownloadButton_clicked();
    void on_cancelDownloadButton_clicked();
    void on_clearDownloadsButton_clicked();
    void on_settingsDownloadButton_clicked();
    void on_startUploadButton_clicked();
    void on_newUploadButton_clicked();
    void on_pauseUploadButton_clicked();
    void on_cancelUploadButton_clicked();
    void on_clearUploadButton_clicked();
    void on_settingsUploadButton_clicked();

    void onFilenameSet(const QString &url, const QString &fileName);
    void onDownloadFinished(const QString &url, const QString &fileName);
    void onDownloadProgress(const QString &url, const qint64 bytesReceived, const qint64 bytesTotal, const double percent, const double speed, const QString &unit);
    void onDownloadStatus(const QString &url, const QString &status, const QString &message, const QString &data);

    void onUrlSet(const QString &path, const QString &url);
    void onUploadFinished(const QString &path, const QString &submitUrl);
    void onUploadProgress(const QString &path, const qint64 bytesSent, const qint64 bytesTotal, const double percent, const double speed, const QString &unit);
    void onUploadStatus(const QString &path, const QString &status, const QString &message, const QString &data);

private:
    void loadPlugins();
    void setupUi();
    void setupDownloader();
    void setupUploader();

    int rowOfDownload(const QString &url) const;
    int rowOfUpload(const QString &path) const;

private:

    Ui::MainWindow *ui;
    QSettings *settings;

    DownloadInterface * downloader;
    UploadInterface * uploader;

    QList<DownloadItem> downloads;
    QList<UploadItem> uploads;
};

#endif // MAINWINDOW_H
