#include "User.h"
#include <QUuid>

User::User(QObject *parent)
    : QObject(parent)
    , m_id(QUuid::createUuid().toString(QUuid::WithoutBraces))
{
}

User::User(const QString &id, const QString &name, QObject *parent)
    : QObject(parent)
    , m_id(id)
    , m_name(name)
{
}

QString User::id() const
{
    return m_id;
}

QString User::name() const
{
    return m_name;
}

QString User::email() const
{
    return m_email;
}

QString User::avatar() const
{
    return m_avatar;
}

UserStatus User::status() const
{
    return m_status;
}

QPixmap User::avatarPixmap() const
{
    return m_avatarPixmap;
}

QDateTime User::lastSeen() const
{
    return m_lastSeen;
}

bool User::isHost() const
{
    return m_isHost;
}

bool User::isMuted() const
{
    return m_isMuted;
}

bool User::isCameraOff() const
{
    return m_isCameraOff;
}

bool User::isHandRaised() const
{
    return m_isHandRaised;
}

void User::setName(const QString &name)
{
    if (m_name != name) {
        m_name = name;
        emit nameChanged(name);
    }
}

void User::setEmail(const QString &email)
{
    if (m_email != email) {
        m_email = email;
        emit emailChanged(email);
    }
}

void User::setAvatar(const QString &avatar)
{
    if (m_avatar != avatar) {
        m_avatar = avatar;
        emit avatarChanged(avatar);
    }
}

void User::setStatus(UserStatus status)
{
    if (m_status != status) {
        m_status = status;
        emit statusChanged(status);
    }
}

void User::setAvatarPixmap(const QPixmap &pixmap)
{
    m_avatarPixmap = pixmap;
    emit avatarPixmapChanged(pixmap);
}

void User::setLastSeen(const QDateTime &time)
{
    m_lastSeen = time;
}

void User::setIsHost(bool isHost)
{
    m_isHost = isHost;
}

void User::setIsMuted(bool isMuted)
{
    m_isMuted = isMuted;
}

void User::setIsCameraOff(bool isCameraOff)
{
    m_isCameraOff = isCameraOff;
}

void User::setIsHandRaised(bool raised)
{
    m_isHandRaised = raised;
}

QString User::statusText() const
{
    switch (m_status) {
    case UserStatus::Offline:
        return tr("离线");
    case UserStatus::Online:
        return tr("在线");
    case UserStatus::Busy:
        return tr("忙碌");
    case UserStatus::InMeeting:
        return tr("会议中");
    case UserStatus::Away:
        return tr("离开");
    default:
        return tr("未知");
    }
}

QString User::statusColor() const
{
    switch (m_status) {
    case UserStatus::Online:
    case UserStatus::InMeeting:
        return QStringLiteral("#107c10");  // Green
    case UserStatus::Busy:
        return QStringLiteral("#d83b01");  // Red
    case UserStatus::Away:
        return QStringLiteral("#ffb900");  // Yellow
    case UserStatus::Offline:
    default:
        return QStringLiteral("#666666");  // Gray
    }
}
