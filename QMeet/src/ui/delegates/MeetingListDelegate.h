#ifndef MEETINGLISTDELEGATE_H
#define MEETINGLISTDELEGATE_H

#include <QStyledItemDelegate>

/**
 * @brief 会议列表自定义委托
 * 用于显示会议卡片，包含标题、时间、主持人、参与人数等信息
 */
class MeetingListDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit MeetingListDelegate(QObject *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const override;

private:
    static constexpr int ITEM_HEIGHT = 120;
    static constexpr int PADDING = 16;
};

#endif // MEETINGLISTDELEGATE_H
