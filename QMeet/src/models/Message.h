#ifndef MESSAGE_H
#define MESSAGE_H

#include <QString>
#include <QDateTime>
#include <QObject>

/**
 * @brief 消息类型枚举
 */
enum class MessageType {
    Text,       // 文本消息
    Image,      // 图片消息
    File,       // 文件消息
    System,     // 系统消息
    Emoticon    // 表情消息
};

/**
 * @brief 消息模型类
 * 存储聊天消息信息
 */
class Message : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString id READ id CONSTANT)
    Q_PROPERTY(QString content READ content WRITE setContent NOTIFY contentChanged)
    Q_PROPERTY(QString senderId READ senderId CONSTANT)
    Q_PROPERTY(QString senderName READ senderName CONSTANT)
    Q_PROPERTY(MessageType type READ type CONSTANT)
    Q_PROPERTY(QDateTime timestamp READ timestamp CONSTANT)

public:
    explicit Message(QObject *parent = nullptr);
    Message(const QString &senderId, const QString &senderName, 
            const QString &content, MessageType type = MessageType::Text, 
            QObject *parent = nullptr);
    ~Message() override = default;

    // Getters
    QString id() const;
    QString content() const;
    QString senderId() const;
    QString senderName() const;
    MessageType type() const;
    QDateTime timestamp() const;
    bool isOwn() const;

    // Setters
    void setContent(const QString &content);
    void setTimestamp(const QDateTime &timestamp);
    void setIsOwn(bool isOwn);

    // Utility
    QString displayText() const;
    QString timeText() const;

signals:
    void contentChanged(const QString &content);

private:
    QString m_id;
    QString m_content;
    QString m_senderId;
    QString m_senderName;
    MessageType m_type = MessageType::Text;
    QDateTime m_timestamp;
    bool m_isOwn = false;
};

#endif // MESSAGE_H
