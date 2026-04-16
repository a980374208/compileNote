#include "ParticipantListDelegate.h"
#include <QPainter>
#include <QModelIndex>
#include <QApplication>
#include <QFont>
#include <QStyle>

ParticipantListDelegate::ParticipantListDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}

void ParticipantListDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                                    const QModelIndex &index) const
{
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);

    // Draw background
    QColor bgColor = QColor("#2d2d2d");
    if (option.state & QStyle::State_MouseOver) {
        bgColor = QColor("#3d3d3d");
    }
    if (option.state & QStyle::State_Selected) {
        bgColor = QColor("#4d4d4d");
    }
    painter->fillRect(option.rect, bgColor);

    // Extract data
    QString name = index.data(Qt::DisplayRole).toString();
    QString userId = index.data(Qt::UserRole).toString();
    bool isMuted = index.data(Qt::UserRole + 2).toBool();
    bool isCameraOff = index.data(Qt::UserRole + 3).toBool();
    bool isHost = index.data(Qt::UserRole + 4).toBool();

    int x = option.rect.left() + PADDING;
    int y = option.rect.top();
    int h = option.rect.height();

    // Draw avatar circle
    QRect avatarRect(x, y + (h - 32) / 2, 32, 32);
    painter->fillRect(avatarRect, QColor("#0078d4"));
    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor("#0078d4"));
    painter->drawEllipse(avatarRect);

    // Draw avatar initial
    painter->setPen(QColor("white"));
    painter->setFont(QFont("Arial", 12, QFont::Bold));
    painter->drawText(avatarRect, Qt::AlignCenter, name.isEmpty() ? "?" : QString(name.at(0)).toUpper());

    // Draw name
    int nameX = x + 32 + PADDING + 4;
    painter->setPen(QColor("white"));
    painter->setFont(QFont("Arial", 12));
    QRect nameRect(nameX, y, option.rect.width() - nameX - PADDING - 100, h);
    painter->drawText(nameRect, Qt::AlignVCenter, name);

    // Draw status and host badge on the right
    int rightX = option.rect.right() - PADDING;

    // Host badge
    if (isHost) {
        painter->setFont(QFont("Arial", 9));
        painter->setPen(QColor("#333333"));
        QRect hostRect(rightX - 50, y + (h - 20) / 2, 50, 20);
        painter->fillRect(hostRect, QColor("#ffb900"));
        painter->drawText(hostRect, Qt::AlignCenter, "主持人");
        rightX -= 60;
    }

    // Status icons
    QString statusIcons;
    if (isMuted) {
        statusIcons += "🔇 ";
    }
    if (isCameraOff) {
        statusIcons += "📷";
    }

    if (!statusIcons.isEmpty()) {
        painter->setFont(QFont("Arial", 11));
        painter->setPen(QColor("white"));
        QRect statusRect(rightX - 40, y, 40, h);
        painter->drawText(statusRect, Qt::AlignCenter | Qt::AlignRight, statusIcons);
    }

    painter->restore();
}

QSize ParticipantListDelegate::sizeHint(const QStyleOptionViewItem &option,
                                         const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);
    return QSize(option.rect.width(), ITEM_HEIGHT);
}
