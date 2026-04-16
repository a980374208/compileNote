#ifndef MEETINGLISTMODEL_H
#define MEETINGLISTMODEL_H

#include <QAbstractListModel>
#include <QList>

/**
 * @brief 会议信息结构体
 */
struct MeetingInfo {
    QString id;
    QString title;
    QString dateTime;
    QString host;
    int participantCount = 0;
    bool isOngoing = false;
};

/**
 * @brief 会议列表数据模型
 * 基于 QAbstractListModel，提供会议数据管理
 */
class MeetingListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit MeetingListModel(QObject *parent = nullptr);

    // Model interface
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Data management
    void addMeeting(const MeetingInfo &meeting);
    void removeMeeting(int row);
    void clear();
    void setMeetings(const QList<MeetingInfo> &meetings);

    // Getters
    MeetingInfo getMeeting(int row) const;
    QList<MeetingInfo> getMeetings() const { return m_meetings; }

private:
    QList<MeetingInfo> m_meetings;
};

#endif // MEETINGLISTMODEL_H
