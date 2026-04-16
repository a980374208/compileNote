#include "Message.h"
#include <QUuid>

Message::Message(QObject *parent)
    : QObject(parent)
    , m_id(QUuid::createUuid().toString(QUuid::WithoutBraces))
    , m_timestamp(QDateTime::currentDateTime())
{
}

Message::Message(const QString &senderId, const QString &senderName, 
                 const QString &content, MessageType type, QObject *parent)
    : QObject(parent)
    , m_id(QUuid::createUuid().toString(QUuid::WithoutBraces))
    , m_content(content)
    , m_senderId(senderId)
    , m_senderName(senderName)
    , m_type(type)
    , m_timestamp(QDateTime::currentDateTime())
{
}

QString Message::id() const
{
    return m_id;
}

QString Message::content() const
{
    return m_content;
}

QString Message::senderId() const
{
    return m_senderId;
}

QString Message::senderName() const
{
    return m_senderName;
}

MessageType Message::type() const
{
    return m_type;
}

QDateTime Message::timestamp() const
{
    return m_timestamp;
}

bool Message::isOwn() const
{
    return m_isOwn;
}

void Message::setContent(const QString &content)
{
    if (m_content != content) {
        m_content = content;
        emit contentChanged(content);
    }
}

void Message::setTimestamp(const QDateTime &timestamp)
{
    m_timestamp = timestamp;
}

void Message::setIsOwn(bool isOwn)
{
    m_isOwn = isOwn;
}

QString Message::displayText() const
{
    switch (m_type) {
    case MessageType::System:
        return tr("[系统] %1").arg(m_content);
    case MessageType::Image:
        return tr("[图片]");
    case MessageType::File:
        return tr("[文件] %1").arg(m_content);
    default:
        return m_content;
    }
}

QString Message::timeText() const
{
    QDateTime now = QDateTime::currentDateTime();
    QDateTime date = m_timestamp;
    
    if (date.date() == now.date()) {
        // Today, show time only
        return date.toString("hh:mm");
    } else if (date.daysTo(now) == 1) {
        // Yesterday
        return tr("昨天 %1").arg(date.toString("hh:mm"));
    } else {
        // Show full date
        return date.toString("MM-dd hh:mm");
    }
}
