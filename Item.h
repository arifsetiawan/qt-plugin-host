#ifndef ITEM_H
#define ITEM_H

#include <QString>

/**
 * Download item
 */
struct DownloadItem {
    QString url;
    QString path;
    int progress;
    QString status;
};

/**
 * Upload item
 */
struct UploadItem {
    QString path;
    QString url;
    int progress;
    QString status;
};

#endif // ITEM_H
