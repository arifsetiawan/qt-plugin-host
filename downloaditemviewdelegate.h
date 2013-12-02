#ifndef DOWNLOADITEMVIEWDELEGATE_H
#define DOWNLOADITEMVIEWDELEGATE_H

#include <QtGui>
#include <QApplication>
#include "mainwindow.h"

// DownloadItemViewDelegate is used to draw the progress bars.
class DownloadItemViewDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    inline DownloadItemViewDelegate(MainWindow *mainWindow) : QItemDelegate(mainWindow) {}

    inline void paint(QPainter *painter, const QStyleOptionViewItem &option,
                      const QModelIndex &index ) const
    {
        if (index.column() != 2) {
            QItemDelegate::paint(painter, option, index);
            return;
        }

        // Set up a QStyleOptionProgressBar to precisely mimic the
        // environment of a progress bar.
        QStyleOptionProgressBar progressBarOption;
        progressBarOption.state = QStyle::State_Enabled;
        progressBarOption.direction = QApplication::layoutDirection();
        progressBarOption.rect = option.rect;
        progressBarOption.fontMetrics = QApplication::fontMetrics();
        progressBarOption.minimum = 0;
        progressBarOption.maximum = 100;
        progressBarOption.textAlignment = Qt::AlignCenter;
        progressBarOption.textVisible = true;

        // Set the progress and text values of the style option.
        int progress = qobject_cast<MainWindow *>(parent())->downloadForRow(index.row()).progress;
        progressBarOption.progress = progress < 0 ? 0 : progress;
        progressBarOption.text = QString().sprintf("%d%%", progressBarOption.progress);

        // Draw the progress bar onto the view.
        QApplication::style()->drawControl(QStyle::CE_ProgressBar, &progressBarOption, painter);
    }
};

#endif // DOWNLOADITEMVIEWDELEGATE_H
