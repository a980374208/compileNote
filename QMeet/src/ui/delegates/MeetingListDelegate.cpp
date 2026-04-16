#include "MeetingListDelegate.h"
#include <QPainter>
#include <QModelIndex>
#include <QApplication>
#include <QFont>

MeetingListDelegate::MeetingListDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}

void MeetingListDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                                const QModelIndex &index) const
{
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);

    // Draw background
    QColor bgColor = (option.state & QStyle::State_Selected) ? QColor("#e5f3ff") : QColor("#ffffff");
    if (option.state & QStyle::State_MouseOver) {
        bgColor = (option.state & QStyle::State_Selected) ? QColor("#d1e7ff") : QColor("#f5f5f5");
    }
    painter->fillRect(option.rect, bgColor);

    // Draw border
    painter->setPen(QPen(QColor("#f0f0f0"), 1));
    painter->drawRect(option.rect.adjusted(0, 0, -1, -1));

    // Extract data
    QString title = index.data(Qt::DisplayRole).toString();
    QString id = index.data(Qt::UserRole).toString();
    QString dateTime = index.data(Qt::UserRole + 1).toString();
    QString host = index.data(Qt::UserRole + 2).toString();
    int participantCount = index.data(Qt::UserRole + 3).toInt();
    bool isOngoing = index.data(Qt::UserRole + 4).toBool();

    QRect itemRect = option.rect.adjusted(PADDING, PADDING, -PADDING, -PADDING);

    // Draw title
    QFont titleFont = painter->font();
    titleFont.setPointSize(14);
    titleFont.setBold(true);
    painter->setFont(titleFont);
    painter->setPen(QColor("#333333"));
    painter->drawText(itemRect.left(), itemRect.top(), itemRect.width(), 24, Qt::AlignLeft | Qt::AlignTop, title);

    // Draw status badge if ongoing
    if (isOngoing) {
        QRect badgeRect(itemRect.right() - 80, itemRect.top(), 70, 20);
        painter->fillRect(badgeRect, QColor("#107c10"));
        painter->setFont(QFont("Arial", 10));
        painter->setPen(QColor("white"));
        painter->drawText(badgeRect, Qt::AlignCenter, "进行中");
    }

    // Draw info row 1: DateTime and Host
    QFont infoFont = painter->font();
    infoFont.setPointSize(11);
    painter->setFont(infoFont);
    painter->setPen(QColor("#666666"));

    int infoY = itemRect.top() + 30;
    painter->drawText(itemRect.left(), infoY, itemRect.width(), 18,
                     Qt::AlignLeft | Qt::AlignTop,
                     QString("📅 %1  •  👤 %2").arg(dateTime, host));

    // Draw info row 2: Meeting ID and Participant Count
    int secondInfoY = infoY + 22;
    painter->drawText(itemRect.left(), secondInfoY, itemRect.width(), 18,
                     Qt::AlignLeft | Qt::AlignTop,
                     QString("会议号: %1  •  👥 %2 人").arg(id).arg(participantCount));

    painter->restore();
}

QSize MeetingListDelegate::sizeHint(const QStyleOptionViewItem &option,
                                     const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);
    return QSize(option.rect.width(), ITEM_HEIGHT);
}
