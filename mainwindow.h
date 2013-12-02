#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

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

private:
    void loadPlugins();

    Ui::MainWindow *ui;
    QSettings *settings;
};

#endif // MAINWINDOW_H
