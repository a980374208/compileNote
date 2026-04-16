#ifndef MEETINGMANAGER_H
#define MEETINGMANAGER_H

#include <QObject>
#include <QString>
#include "../models/Meeting.h"
#include "../models/User.h"

class NetworkManager;

/**
 * @brief 会议管理器
 * 负责会议的创建、加入、离开等操作
 */
class MeetingManager : public QObject
{
    Q_OBJECT

public:
    explicit MeetingManager(QObject *parent = nullptr);
    ~MeetingManager() override;

    Meeting* currentMeeting() const;
    bool isInMeeting() const;

    void createMeeting(const QString &title, const MeetingSettings &settings = MeetingSettings());
    void joinMeeting(const QString &meetingId, const QString &password = QString());
    void leaveMeeting();
    void endMeeting();

    void muteParticipant(const QString &userId);
    void unmuteParticipant(const QString &userId);
    void removeParticipant(const QString &userId);

signals:
    void meetingCreated(Meeting *meeting);
    void meetingJoined(Meeting *meeting);
    void meetingLeft();
    void meetingEnded();
    void error(const QString &message);

    void participantJoined(User *user);
    void participantLeft(const QString &userId);
    void participantMuted(const QString &userId);
    void participantUnmuted(const QString &userId);

private:
    Meeting *m_currentMeeting = nullptr;
    NetworkManager *m_networkManager = nullptr;
};

#endif // MEETINGMANAGER_H
