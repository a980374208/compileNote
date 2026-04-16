#ifndef CHATWIDGET_H
#define CHATWIDGET_H

#include <QWidget>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QScrollArea>
#include <QVBoxLayout>
#include "../../models/Message.h"

/**
 * @brief 聊天组件
 * 显示聊天消息列表和输入框
 */
class ChatWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ChatWidget(QWidget *parent = nullptr);
    ~ChatWidget() override = default;

    void addMessage(Message *message);
    void addSystemMessage(const QString &content);
    void clearMessages();

signals:
    void messageSent(const QString &content);

private slots:
    void onSendClicked();
    void onEmojiClicked();

private:
    void setupUI();
    void setupConnections();
    void createMessageBubble(Message *message);
    void scrollToBottom();

    QWidget *m_messageContainer = nullptr;
    QVBoxLayout *m_messageLayout = nullptr;
    QScrollArea *m_scrollArea = nullptr;
    QLineEdit *m_inputEdit = nullptr;
    QPushButton *m_sendBtn = nullptr;
    QPushButton *m_emojiBtn = nullptr;

    QList<Message*> m_messages;
};

#endif // CHATWIDGET_H
