#ifndef PARTICIPANTLISTDELEGATE_H
#define PARTICIPANTLISTDELEGATE_H

#include <QStyledItemDelegate>

/**
 * @brief 参与者列表自定义委托
 * 用于显示参与者信息卡片，包含头像、名称、状态、主持人徽章等
 */
class ParticipantListDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit ParticipantListDelegate(QObject *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const override;

private:
    static constexpr int ITEM_HEIGHT = 48;
    static constexpr int PADDING = 8;
};

#endif // PARTICIPANTLISTDELEGATE_H
