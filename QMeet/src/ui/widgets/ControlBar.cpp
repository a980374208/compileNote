#include "ControlBar.h"
#include <QVBoxLayout>
#include <QSpacerItem>

ControlBar::ControlBar(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
    setupConnections();
}

void ControlBar::setMicrophoneEnabled(bool enabled)
{
    m_microphoneEnabled = enabled;
    updateButtonStyle(m_micBtn, !enabled, "#d13438");
    m_micBtn->setText(enabled ? "🎤" : "🔇");
}

void ControlBar::setCameraEnabled(bool enabled)
{
    m_cameraEnabled = enabled;
    updateButtonStyle(m_cameraBtn, !enabled, "#d13438");
    m_cameraBtn->setText(enabled ? "📷" : "📷");
}

void ControlBar::setScreenSharing(bool sharing)
{
    m_screenSharing = sharing;
    updateButtonStyle(m_screenShareBtn, sharing, "#107c10");
    m_screenShareBtn->setText(sharing ? "🖥️" : "🖥️");
}

void ControlBar::setChatVisible(bool visible)
{
    m_chatVisible = visible;
    updateButtonStyle(m_chatBtn, visible, "#0078d4");
}

void ControlBar::setParticipantsVisible(bool visible)
{
    m_participantsVisible = visible;
    updateButtonStyle(m_participantsBtn, visible, "#0078d4");
}

void ControlBar::setupUI()
{
    setObjectName("ControlBar");
    setFixedHeight(80);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(32, 12, 32, 12);
    layout->setSpacing(16);

    // Left spacer
    layout->addStretch();

    // Main controls
    m_micBtn = createControlButton("🎤", "麦克风");
    m_cameraBtn = createControlButton("📷", "摄像头");
    m_screenShareBtn = createControlButton("🖥️", "屏幕共享");
    m_chatBtn = createControlButton("💬", "聊天");
    m_participantsBtn = createControlButton("👥", "参与者");

    layout->addWidget(m_micBtn);
    layout->addWidget(m_cameraBtn);
    layout->addWidget(m_screenShareBtn);
    layout->addWidget(m_chatBtn);
    layout->addWidget(m_participantsBtn);

    // Divider
    QFrame *divider = new QFrame();
    divider->setObjectName("divider");
    divider->setFrameShape(QFrame::VLine);
    divider->setFixedWidth(1);
    layout->addSpacing(8);
    layout->addWidget(divider);
    layout->addSpacing(8);

    // Leave button
    m_leaveBtn = new QPushButton("离开会议");
    m_leaveBtn->setObjectName("leaveBtn");
    m_leaveBtn->setCursor(Qt::PointingHandCursor);

    layout->addWidget(m_leaveBtn);

    // Right spacer
    layout->addStretch();
}

void ControlBar::setupConnections()
{
    connect(m_micBtn, &QPushButton::clicked, this, &ControlBar::onMicClicked);
    connect(m_cameraBtn, &QPushButton::clicked, this, &ControlBar::onCameraClicked);
    connect(m_screenShareBtn, &QPushButton::clicked, this, &ControlBar::onScreenShareClicked);
    connect(m_chatBtn, &QPushButton::clicked, this, &ControlBar::onChatClicked);
    connect(m_participantsBtn, &QPushButton::clicked, this, &ControlBar::onParticipantsClicked);
    connect(m_leaveBtn, &QPushButton::clicked, this, &ControlBar::onLeaveClicked);
}

QPushButton* ControlBar::createControlButton(const QString &icon, const QString &tooltip)
{
    QPushButton *btn = new QPushButton(icon);
    btn->setObjectName("controlBtn");
    btn->setFixedSize(56, 56);
    btn->setToolTip(tooltip);
    btn->setCursor(Qt::PointingHandCursor);
    return btn;
}

void ControlBar::updateButtonStyle(QPushButton *button, bool active, const QString &activeColor)
{
    // Style is applied through stylesheet when object is set to "controlBtn"
    // No dynamic color styling needed for now
    Q_UNUSED(button);
    Q_UNUSED(active);
    Q_UNUSED(activeColor);
}

void ControlBar::onMicClicked()
{
    emit toggleMicrophone();
}

void ControlBar::onCameraClicked()
{
    emit toggleCamera();
}

void ControlBar::onScreenShareClicked()
{
    emit toggleScreenShare();
}

void ControlBar::onChatClicked()
{
    emit toggleChat();
}

void ControlBar::onParticipantsClicked()
{
    emit toggleParticipants();
}

void ControlBar::onLeaveClicked()
{
    emit leaveMeeting();
}
