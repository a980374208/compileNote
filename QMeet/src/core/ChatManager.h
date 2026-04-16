#ifndef CHATMANAGER_H
#define CHATMANAGER_H

#include <QObject>
#include <QList>
#include "../models/Message.h"

/**
 * @brief 聊天管理器
 * 负责聊天消息的发送和接收
 */
class ChatManager : public QObject
{
    Q_OBJECT

public:
    explicit ChatManager(QObject *parent = nullptr);
    ~ChatManager() override = default;

    QList<Message*> messages() const;
    void clearMessages();

    void sendMessage(const QString &content);
    void sendImage(const QString &imagePath);
    void sendFile(const QString &filePath);

signals:
    void messageSent(Message *message);
    void messageReceived(Message *message);
    void messageFailed(const QString &error);

private:
    QList<Message*> m_messages;
};

#endif // CHATMANAGER_H
