#include "Meeting.h"
#include <QUuid>
#include <QRandomGenerator>

Meeting::Meeting(QObject *parent)
    : QObject(parent)
    , m_id(QUuid::createUuid().toString(QUuid::WithoutBraces))
    , m_meetingNumber(QString::number(QRandomGenerator::global()->bounded(100000000, 999999999)))
{
}

Meeting::Meeting(const QString &id, const QString &title, const QString &hostId, QObject *parent)
    : QObject(parent)
    , m_id(id)
    , m_title(title)
    , m_hostId(hostId)
    , m_meetingNumber(QString::number(QRandomGenerator::global()->bounded(100000000, 999999999)))
{
}

QString Meeting::id() const
{
    return m_id;
}

QString Meeting::title() const
{
    return m_title;
}

QString Meeting::hostId() const
{
    return m_hostId;
}

MeetingStatus Meeting::status() const
{
    return m_status;
}

QDateTime Meeting::startTime() const
{
    return m_startTime;
}

QDateTime Meeting::endTime() const
{
    return m_endTime;
}

QString Meeting::meetingUrl() const
{
    return m_meetingUrl;
}

QString Meeting::meetingNumber() const
{
    return m_meetingNumber;
}

int Meeting::participantCount() const
{
    return m_participants.size();
}

QList<User*> Meeting::participants() const
{
    return m_participants;
}

MeetingSettings Meeting::settings() const
{
    return m_settings;
}

User* Meeting::participantById(const QString &userId) const
{
    for (User *user : m_participants) {
        if (user->id() == userId) {
            return user;
        }
    }
    return nullptr;
}

void Meeting::setTitle(const QString &title)
{
    if (m_title != title) {
        m_title = title;
        emit titleChanged(title);
    }
}

void Meeting::setStatus(MeetingStatus status)
{
    if (m_status != status) {
        m_status = status;
        emit statusChanged(status);
    }
}

void Meeting::setStartTime(const QDateTime &time)
{
    m_startTime = time;
}

void Meeting::setEndTime(const QDateTime &time)
{
    m_endTime = time;
}

void Meeting::setMeetingUrl(const QString &url)
{
    m_meetingUrl = url;
}

void Meeting::setMeetingNumber(const QString &number)
{
    m_meetingNumber = number;
}

void Meeting::setSettings(const MeetingSettings &settings)
{
    m_settings = settings;
}

void Meeting::addParticipant(User *user)
{
    if (user && !isParticipant(user->id())) {
        m_participants.append(user);
        emit participantsChanged();
        emit participantJoined(user);
    }
}

void Meeting::removeParticipant(const QString &userId)
{
    for (int i = 0; i < m_participants.size(); ++i) {
        if (m_participants[i]->id() == userId) {
            m_participants.removeAt(i);
            emit participantsChanged();
            emit participantLeft(userId);
            break;
        }
    }
}

void Meeting::clearParticipants()
{
    m_participants.clear();
    emit participantsChanged();
}

QString Meeting::statusText() const
{
    switch (m_status) {
    case MeetingStatus::Waiting:
        return tr("等待开始");
    case MeetingStatus::InProgress:
        return tr("进行中");
    case MeetingStatus::Paused:
        return tr("已暂停");
    case MeetingStatus::Ended:
        return tr("已结束");
    default:
        return tr("未知");
    }
}

int Meeting::duration() const
{
    if (!m_startTime.isValid()) {
        return 0;
    }
    
    QDateTime endTime = m_endTime.isValid() ? m_endTime : QDateTime::currentDateTime();
    return static_cast<int>(m_startTime.secsTo(endTime));
}

bool Meeting::isParticipant(const QString &userId) const
{
    for (const User *user : m_participants) {
        if (user->id() == userId) {
            return true;
        }
    }
    return false;
}
