#include "ChatWidget.h"
#include <QLabel>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QScrollBar>

ChatWidget::ChatWidget(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
    setupConnections();
}

void ChatWidget::addMessage(Message *message)
{
    m_messages.append(message);
    createMessageBubble(message);
    scrollToBottom();
}

void ChatWidget::addSystemMessage(const QString &content)
{
    Message *msg = new Message();
    msg->setContent(content);
    // msg->setType(MessageType::System);
    addMessage(msg);
}

void ChatWidget::clearMessages()
{
    m_messages.clear();
    // Clear message container
    QLayoutItem *item;
    while ((item = m_messageLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
}

void ChatWidget::setupUI()
{
    setObjectName("ChatWidget");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // Header
    QWidget *header = new QWidget();
    header->setObjectName("chatHeader");
    header->setFixedHeight(48);
    QHBoxLayout *headerLayout = new QHBoxLayout(header);
    headerLayout->setContentsMargins(16, 0, 16, 0);

    QLabel *titleLabel = new QLabel("💬 聊天");
    titleLabel->setObjectName("chatTitleLabel");
    headerLayout->addWidget(titleLabel);
    headerLayout->addStretch();

    mainLayout->addWidget(header);

    // Message area
    m_scrollArea = new QScrollArea();
    m_scrollArea->setObjectName("messageScrollArea");
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    m_messageContainer = new QWidget();
    m_messageLayout = new QVBoxLayout(m_messageContainer);
    m_messageLayout->setContentsMargins(8, 8, 8, 8);
    m_messageLayout->setSpacing(8);
    m_messageLayout->addStretch();

    m_scrollArea->setWidget(m_messageContainer);
    mainLayout->addWidget(m_scrollArea, 1);

    // Input area
    QWidget *inputArea = new QWidget();
    inputArea->setObjectName("chatInputArea");
    inputArea->setFixedHeight(56);
    QHBoxLayout *inputLayout = new QHBoxLayout(inputArea);
    inputLayout->setContentsMargins(8, 8, 8, 8);
    inputLayout->setSpacing(8);

    m_emojiBtn = new QPushButton("😊");
    m_emojiBtn->setObjectName("emojiBtn");
    m_emojiBtn->setFixedSize(32, 32);

    m_inputEdit = new QLineEdit();
    m_inputEdit->setObjectName("inputEdit");
    m_inputEdit->setPlaceholderText("输入消息...");

    m_sendBtn = new QPushButton("发送");
    m_sendBtn->setObjectName("sendBtn");
    m_sendBtn->setFixedSize(60, 32);

    inputLayout->addWidget(m_emojiBtn);
    inputLayout->addWidget(m_inputEdit, 1);
    inputLayout->addWidget(m_sendBtn);

    mainLayout->addWidget(inputArea);

    // Add welcome message
    addSystemMessage("欢迎使用 QMeet 聊天功能");
}

void ChatWidget::setupConnections()
{
    connect(m_sendBtn, &QPushButton::clicked, this, &ChatWidget::onSendClicked);
    connect(m_inputEdit, &QLineEdit::returnPressed, this, &ChatWidget::onSendClicked);
    connect(m_emojiBtn, &QPushButton::clicked, this, &ChatWidget::onEmojiClicked);
}

void ChatWidget::createMessageBubble(Message *message)
{
    QWidget *bubbleWidget = new QWidget();
    QHBoxLayout *bubbleLayout = new QHBoxLayout(bubbleWidget);
    bubbleLayout->setContentsMargins(0, 0, 0, 0);
    bubbleLayout->setSpacing(8);

    QLabel *messageLabel = new QLabel();
    messageLabel->setText(message->displayText());
    messageLabel->setWordWrap(true);
    messageLabel->setMaximumWidth(250);

    if (message->isOwn()) {
        // Right-aligned for own messages
        bubbleLayout->addStretch();
        messageLabel->setStyleSheet(R"(
            background-color: #0078d4;
            color: white;
            border-radius: 12px;
            padding: 8px 12px;
            font-size: 14px;
        )");
        bubbleLayout->addWidget(messageLabel);
    } else {
        // Left-aligned for others' messages
        QLabel *senderLabel = new QLabel(message->senderName());
        senderLabel->setStyleSheet("color: #999999; font-size: 12px;");
        
        QVBoxLayout *leftLayout = new QVBoxLayout();
        leftLayout->addWidget(senderLabel);
        
        messageLabel->setStyleSheet(R"(
            background-color: #3d3d3d;
            color: white;
            border-radius: 12px;
            padding: 8px 12px;
            font-size: 14px;
        )");
        leftLayout->addWidget(messageLabel);
        
        bubbleLayout->addLayout(leftLayout);
        bubbleLayout->addStretch();
    }

    m_messageLayout->insertWidget(m_messageLayout->count() - 1, bubbleWidget);
}

void ChatWidget::scrollToBottom()
{
    QScrollBar *scrollBar = m_scrollArea->verticalScrollBar();
    scrollBar->setValue(scrollBar->maximum());
}

void ChatWidget::onSendClicked()
{
    QString text = m_inputEdit->text().trimmed();
    if (!text.isEmpty()) {
        Message *msg = new Message("local", "我", text);
        msg->setIsOwn(true);
        addMessage(msg);

        m_inputEdit->clear();
        emit messageSent(text);
    }
}

void ChatWidget::onEmojiClicked()
{
    // TODO: Show emoji picker
}
