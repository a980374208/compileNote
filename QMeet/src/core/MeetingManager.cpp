#include "MeetingManager.h"
#include "NetworkManager.h"
#include <QUuid>

MeetingManager::MeetingManager(QObject *parent)
    : QObject(parent)
    , m_networkManager(new NetworkManager(this))
{
}

MeetingManager::~MeetingManager()
{
    if (m_currentMeeting) {
        delete m_currentMeeting;
    }
}

Meeting* MeetingManager::currentMeeting() const
{
    return m_currentMeeting;
}

bool MeetingManager::isInMeeting() const
{
    return m_currentMeeting != nullptr && 
           m_currentMeeting->status() == MeetingStatus::InProgress;
}

void MeetingManager::createMeeting(const QString &title, const MeetingSettings &settings)
{
    // Create new meeting
    QString meetingId = QUuid::createUuid().toString(QUuid::WithoutBraces);
    m_currentMeeting = new Meeting(meetingId, title, "local_user", this);
    m_currentMeeting->setSettings(settings);
    m_currentMeeting->setStatus(MeetingStatus::InProgress);
    m_currentMeeting->setStartTime(QDateTime::currentDateTime());

    emit meetingCreated(m_currentMeeting);
    emit meetingJoined(m_currentMeeting);
}

void MeetingManager::joinMeeting(const QString &meetingId, const QString &password)
{
    Q_UNUSED(password);

    // TODO: Implement actual network call to join meeting
    m_currentMeeting = new Meeting(meetingId, "会议", "host_id", this);
    m_currentMeeting->setStatus(MeetingStatus::InProgress);
    m_currentMeeting->setStartTime(QDateTime::currentDateTime());

    emit meetingJoined(m_currentMeeting);
}

void MeetingManager::leaveMeeting()
{
    if (m_currentMeeting) {
        m_currentMeeting->setStatus(MeetingStatus::Ended);
        m_currentMeeting->setEndTime(QDateTime::currentDateTime());
        
        emit meetingLeft();
        
        delete m_currentMeeting;
        m_currentMeeting = nullptr;
    }
}

void MeetingManager::endMeeting()
{
    if (m_currentMeeting) {
        m_currentMeeting->setStatus(MeetingStatus::Ended);
        m_currentMeeting->setEndTime(QDateTime::currentDateTime());
        
        emit meetingEnded();
        emit meetingLeft();
        
        delete m_currentMeeting;
        m_currentMeeting = nullptr;
    }
}

void MeetingManager::muteParticipant(const QString &userId)
{
    if (m_currentMeeting) {
        User *participant = m_currentMeeting->participantById(userId);
        if (participant) {
            participant->setIsMuted(true);
            emit participantMuted(userId);
        }
    }
}

void MeetingManager::unmuteParticipant(const QString &userId)
{
    if (m_currentMeeting) {
        User *participant = m_currentMeeting->participantById(userId);
        if (participant) {
            participant->setIsMuted(false);
            emit participantUnmuted(userId);
        }
    }
}

void MeetingManager::removeParticipant(const QString &userId)
{
    if (m_currentMeeting) {
        m_currentMeeting->removeParticipant(userId);
        emit participantLeft(userId);
    }
}
