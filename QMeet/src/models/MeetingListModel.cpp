#include "MeetingListModel.h"

MeetingListModel::MeetingListModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int MeetingListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0;
    }
    return m_meetings.count();
}

QVariant MeetingListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_meetings.count()) {
        return QVariant();
    }

    const MeetingInfo &meeting = m_meetings.at(index.row());

    switch (role) {
        case Qt::DisplayRole:
        case Qt::EditRole:
            return meeting.title;
        case Qt::UserRole:
            return meeting.id;
        case Qt::UserRole + 1:
            return meeting.dateTime;
        case Qt::UserRole + 2:
            return meeting.host;
        case Qt::UserRole + 3:
            return meeting.participantCount;
        case Qt::UserRole + 4:
            return meeting.isOngoing;
        default:
            return QVariant();
    }
}

void MeetingListModel::addMeeting(const MeetingInfo &meeting)
{
    beginInsertRows(QModelIndex(), m_meetings.count(), m_meetings.count());
    m_meetings.append(meeting);
    endInsertRows();
}

void MeetingListModel::removeMeeting(int row)
{
    if (row < 0 || row >= m_meetings.count()) {
        return;
    }

    beginRemoveRows(QModelIndex(), row, row);
    m_meetings.removeAt(row);
    endRemoveRows();
}

void MeetingListModel::clear()
{
    beginResetModel();
    m_meetings.clear();
    endResetModel();
}

void MeetingListModel::setMeetings(const QList<MeetingInfo> &meetings)
{
    beginResetModel();
    m_meetings = meetings;
    endResetModel();
}

MeetingInfo MeetingListModel::getMeeting(int row) const
{
    if (row < 0 || row >= m_meetings.count()) {
        return MeetingInfo();
    }
    return m_meetings.at(row);
}
