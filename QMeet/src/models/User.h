#ifndef USER_H
#define USER_H

#include <QString>
#include <QPixmap>
#include <QDateTime>
#include <QObject>

/**
 * @brief 用户状态枚举
 */
enum class UserStatus {
    Offline,    // 离线
    Online,     // 在线
    Busy,       // 忙碌
    InMeeting,  // 会议中
    Away        // 离开
};

/**
 * @brief 用户模型类
 * 存储用户的基本信息
 */
class User : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString id READ id CONSTANT)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString email READ email WRITE setEmail NOTIFY emailChanged)
    Q_PROPERTY(QString avatar READ avatar WRITE setAvatar NOTIFY avatarChanged)
    Q_PROPERTY(UserStatus status READ status WRITE setStatus NOTIFY statusChanged)

public:
    explicit User(QObject *parent = nullptr);
    User(const QString &id, const QString &name, QObject *parent = nullptr);
    ~User() override = default;

    // Getters
    QString id() const;
    QString name() const;
    QString email() const;
    QString avatar() const;
    UserStatus status() const;
    QPixmap avatarPixmap() const;
    QDateTime lastSeen() const;
    bool isHost() const;
    bool isMuted() const;
    bool isCameraOff() const;
    bool isHandRaised() const;

    // Setters
    void setName(const QString &name);
    void setEmail(const QString &email);
    void setAvatar(const QString &avatar);
    void setStatus(UserStatus status);
    void setAvatarPixmap(const QPixmap &pixmap);
    void setLastSeen(const QDateTime &time);
    void setIsHost(bool isHost);
    void setIsMuted(bool isMuted);
    void setIsCameraOff(bool isCameraOff);
    void setIsHandRaised(bool raised);

    // Utility methods
    QString statusText() const;
    QString statusColor() const;

signals:
    void nameChanged(const QString &name);
    void emailChanged(const QString &email);
    void avatarChanged(const QString &avatar);
    void statusChanged(UserStatus status);
    void avatarPixmapChanged(const QPixmap &pixmap);

private:
    QString m_id;
    QString m_name;
    QString m_email;
    QString m_avatar;
    QPixmap m_avatarPixmap;
    UserStatus m_status = UserStatus::Offline;
    QDateTime m_lastSeen;
    bool m_isHost = false;
    bool m_isMuted = false;
    bool m_isCameraOff = true;
    bool m_isHandRaised = false;
};

#endif // USER_H
