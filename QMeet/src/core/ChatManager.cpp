#include "ChatManager.h"
#include <QDateTime>

ChatManager::ChatManager(QObject *parent)
    : QObject(parent)
{
}

QList<Message*> ChatManager::messages() const
{
    return m_messages;
}

void ChatManager::clearMessages()
{
    m_messages.clear();
}

void ChatManager::sendMessage(const QString &content)
{
    Message *msg = new Message("local", "我", content, MessageType::Text, this);
    msg->setTimestamp(QDateTime::currentDateTime());
    msg->setIsOwn(true);
    
    m_messages.append(msg);
    emit messageSent(msg);
}

void ChatManager::sendImage(const QString &imagePath)
{
    Message *msg = new Message("local", "我", imagePath, MessageType::Image, this);
    msg->setTimestamp(QDateTime::currentDateTime());
    msg->setIsOwn(true);
    
    m_messages.append(msg);
    emit messageSent(msg);
}

void ChatManager::sendFile(const QString &filePath)
{
    Message *msg = new Message("local", "我", filePath, MessageType::File, this);
    msg->setTimestamp(QDateTime::currentDateTime());
    msg->setIsOwn(true);
    
    m_messages.append(msg);
    emit messageSent(msg);
}
