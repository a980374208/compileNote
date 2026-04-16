#ifndef MEETING_H
#define MEETING_H

#include <QString>
#include <QDateTime>
#include <QObject>
#include <QList>
#include "User.h"

/**
 * @brief 会议状态枚举
 */
enum class MeetingStatus {
    Waiting,    // 等待开始
    InProgress, // 进行中
    Paused,     // 已暂停
    Ended       // 已结束
};

/**
 * @brief 会议设置
 */
struct MeetingSettings {
    bool enableVideo = true;
    bool enableAudio = true;
    bool enableChat = true;
    bool enableScreenShare = true;
    bool enableWhiteboard = false;
    bool enableRecording = false;
    bool isPasswordProtected = false;
    QString password;
    int maxParticipants = 100;
    bool waitingRoomEnabled = false;
};

/**
 * @brief 会议模型类
 * 存储会议信息和参与者列表
 */
class Meeting : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString id READ id CONSTANT)
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString hostId READ hostId CONSTANT)
    Q_PROPERTY(MeetingStatus status READ status WRITE setStatus NOTIFY statusChanged)
    Q_PROPERTY(int participantCount READ participantCount NOTIFY participantsChanged)

public:
    explicit Meeting(QObject *parent = nullptr);
    Meeting(const QString &id, const QString &title, const QString &hostId, QObject *parent = nullptr);
    ~Meeting() override = default;

    // Getters
    QString id() const;
    QString title() const;
    QString hostId() const;
    MeetingStatus status() const;
    QDateTime startTime() const;
    QDateTime endTime() const;
    QString meetingUrl() const;
    QString meetingNumber() const;
    int participantCount() const;
    QList<User*> participants() const;
    MeetingSettings settings() const;
    User* participantById(const QString &userId) const;

    // Setters
    void setTitle(const QString &title);
    void setStatus(MeetingStatus status);
    void setStartTime(const QDateTime &time);
    void setEndTime(const QDateTime &time);
    void setMeetingUrl(const QString &url);
    void setMeetingNumber(const QString &number);
    void setSettings(const MeetingSettings &settings);

    // Participant management
    void addParticipant(User *user);
    void removeParticipant(const QString &userId);
    void clearParticipants();

    // Utility methods
    QString statusText() const;
    int duration() const;  // in seconds
    bool isParticipant(const QString &userId) const;

signals:
    void titleChanged(const QString &title);
    void statusChanged(MeetingStatus status);
    void participantsChanged();
    void participantJoined(User *user);
    void participantLeft(const QString &userId);

private:
    QString m_id;
    QString m_title;
    QString m_hostId;
    MeetingStatus m_status = MeetingStatus::Waiting;
    QDateTime m_startTime;
    QDateTime m_endTime;
    QString m_meetingUrl;
    QString m_meetingNumber;
    QList<User*> m_participants;
    MeetingSettings m_settings;
};

#endif // MEETING_H
