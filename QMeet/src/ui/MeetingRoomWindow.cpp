#include "MeetingRoomWindow.h"
#include "widgets/VideoWidget.h"
#include "widgets/ControlBar.h"
#include "widgets/ChatWidget.h"
#include "widgets/ParticipantWidget.h"
#include "widgets/ScreenShareWidget.h"
#include "../core/ScreenShareManager.h"
#include "../core/MeetingManager.h"
#include "../app/Application.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QSplitter>
#include <QStackedWidget>
#include <QMessageBox>
#include <QImage>
#include <QSizePolicy>

MeetingRoomWindow::MeetingRoomWindow(QWidget *parent)
    : QWidget(parent)
    , m_screenShareManager(new ScreenShareManager(this))
{
    setupUI();
    setupConnections();
    initializeMediaDevices();
}

MeetingRoomWindow::~MeetingRoomWindow()
{
    if (m_durationTimer) {
        m_durationTimer->stop();
    }
}

void MeetingRoomWindow::joinMeeting(const QString &meetingId)
{
    Q_UNUSED(meetingId);
    // TODO: Implement actual join logic
    m_meetingElapsed.start();
    m_durationTimer->start(1000);
}

void MeetingRoomWindow::setMeeting(Meeting *meeting)
{
    m_meeting = meeting;
    if (m_meeting) {
        m_meetingTitleLabel->setText(m_meeting->title());
        m_meetingNumberLabel->setText(QString("会议号: %1").arg(m_meeting->meetingNumber()));
    }
}

void MeetingRoomWindow::setupUI()
{
    setObjectName("MeetingRoomWindow");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // === Header ===
    QWidget *header = new QWidget();
    header->setObjectName("meetingHeader");
    header->setFixedHeight(56);
    QHBoxLayout *headerLayout = new QHBoxLayout(header);
    headerLayout->setContentsMargins(16, 0, 16, 0);

    m_meetingTitleLabel = new QLabel("会议主题");
    m_meetingTitleLabel->setObjectName("meetingTitleLabel");

    m_meetingNumberLabel = new QLabel("会议号: ---");
    m_meetingNumberLabel->setObjectName("meetingNumberLabel");

    m_durationLabel = new QLabel("00:00:00");
    m_durationLabel->setObjectName("durationLabel");

    m_moreBtn = new QPushButton("⋯");
    m_moreBtn->setObjectName("moreBtn");
    m_moreBtn->setFixedSize(32, 32);

    headerLayout->addWidget(m_meetingTitleLabel);
    headerLayout->addSpacing(8);
    headerLayout->addWidget(m_meetingNumberLabel);
    headerLayout->addStretch();
    headerLayout->addWidget(m_durationLabel);
    headerLayout->addSpacing(16);
    headerLayout->addWidget(m_moreBtn);

    mainLayout->addWidget(header);

    // === Content Area ===
    QHBoxLayout *contentLayout = new QHBoxLayout();
    contentLayout->setSpacing(0);

    // Video grid area
    m_videoArea = new QWidget();
    m_videoArea->setObjectName("videoArea");
    QGridLayout *videoGrid = new QGridLayout(m_videoArea);
    videoGrid->setSpacing(4);
    videoGrid->setContentsMargins(0, 0, 0, 0);

    // Local video (main view)
    m_localVideoWidget = new VideoWidget();
    m_localVideoWidget->setIsLocal(true);
    m_localVideoWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    videoGrid->addWidget(m_localVideoWidget, 0, 0);

    // Screen share widget (initially hidden)
    m_screenShareWidget = new ScreenShareWidget();
    m_screenShareWidget->setVisible(false);
    m_screenShareWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    videoGrid->addWidget(m_screenShareWidget, 0, 0);
    
    // Make grid expand to fill available space
    videoGrid->setRowStretch(0, 1);
    videoGrid->setColumnStretch(0, 1);

    // Remote videos (small views)
    for (int i = 0; i < 5; ++i) {
        VideoWidget *remoteVideo = new VideoWidget();
        remoteVideo->setIsLocal(false);
        remoteVideo->setVisible(false);
        m_remoteVideoWidgets.append(remoteVideo);
    }

    contentLayout->addWidget(m_videoArea, 1);

    // Side panel (chat / participants)
    m_sidePanel = new QWidget();
    m_sidePanel->setObjectName("sidePanel");
    m_sidePanel->setFixedWidth(0);
    QVBoxLayout *sideLayout = new QVBoxLayout(m_sidePanel);
    sideLayout->setContentsMargins(0, 0, 0, 0);

    m_chatWidget = new ChatWidget();
    m_participantWidget = new ParticipantWidget();
    m_participantWidget->setVisible(false);
    sideLayout->addWidget(m_chatWidget);
    sideLayout->addWidget(m_participantWidget);

    contentLayout->addWidget(m_sidePanel);

    mainLayout->addLayout(contentLayout, 1);

    // === Control Bar ===
    m_controlBar = new ControlBar();
    mainLayout->addWidget(m_controlBar);

    // Duration timer
    m_durationTimer = new QTimer(this);
}

void MeetingRoomWindow::setupConnections()
{
    connect(m_controlBar, &ControlBar::toggleMicrophone, this, &MeetingRoomWindow::onToggleMicrophone);
    connect(m_controlBar, &ControlBar::toggleCamera, this, &MeetingRoomWindow::onToggleCamera);
    connect(m_controlBar, &ControlBar::toggleScreenShare, this, &MeetingRoomWindow::onToggleScreenShare);
    connect(m_controlBar, &ControlBar::toggleChat, this, &MeetingRoomWindow::onToggleChat);
    connect(m_controlBar, &ControlBar::toggleParticipants, this, &MeetingRoomWindow::onToggleParticipants);
    connect(m_controlBar, &ControlBar::leaveMeeting, this, &MeetingRoomWindow::onLeaveMeeting);
    connect(m_moreBtn, &QPushButton::clicked, this, &MeetingRoomWindow::onMoreOptionsClicked);
    connect(m_durationTimer, &QTimer::timeout, this, &MeetingRoomWindow::updateMeetingDuration);

    // Screen share connections
    connect(this, &MeetingRoomWindow::startScreenShare, this, [this]() {
        m_screenShareManager->startScreenShare();
    });
    connect(this, &MeetingRoomWindow::stopScreenShare, this, [this]() {
        m_screenShareManager->stopShare();
    });
    connect(m_screenShareManager, &ScreenShareManager::shareStarted, this, [this]() {
        m_screenSharing = true;
        m_controlBar->setScreenSharing(true);
    });
    connect(m_screenShareManager, &ScreenShareManager::shareStopped, this, [this]() {
        m_screenSharing = false;
        m_controlBar->setScreenSharing(false);
    });
    connect(m_screenShareManager, &ScreenShareManager::error, this, [this](const QString &msg) {
        QMessageBox::warning(this, "屏幕共享错误", msg);
        m_screenSharing = false;
        m_controlBar->setScreenSharing(false);
        m_screenShareWidget->setVisible(false);
        m_localVideoWidget->setVisible(true);
    });

    // Connect frame to screen share widget
    connect(m_screenShareManager, &ScreenShareManager::frameReady, this, [this](const QImage &frame) {
        qDebug()<<"<<<<<<<<<<<<<<m_videoArea "<<this->m_videoArea->rect()<<"local"<<this->rect();
        if (m_screenShareWidget && m_screenSharing) {
            m_screenShareWidget->setScreenImage(frame);
        }
    });
    connect(m_screenShareManager, &ScreenShareManager::shareStarted, this, [this]() {
        m_screenShareWidget->setSharing(true);
        m_screenShareWidget->setVisible(true);
        m_localVideoWidget->setVisible(false);
    });
    connect(m_screenShareManager, &ScreenShareManager::shareStopped, this, [this]() {
        m_screenShareWidget->setSharing(false);
        m_screenShareWidget->setVisible(false);
        m_localVideoWidget->setVisible(true);
    });
    connect(m_screenShareWidget, &ScreenShareWidget::stopSharing, this, [this]() {
        m_screenShareManager->stopShare();
    });
}

void MeetingRoomWindow::initializeMediaDevices()
{
    // TODO: Initialize actual media devices using Qt Multimedia
}

void MeetingRoomWindow::onToggleMicrophone()
{
    m_microphoneEnabled = !m_microphoneEnabled;
    m_controlBar->setMicrophoneEnabled(m_microphoneEnabled);

    if (m_localVideoWidget) {
        m_localVideoWidget->setMuted(!m_microphoneEnabled);
    }

    // TODO: Actually mute/unmute audio
}

void MeetingRoomWindow::onToggleCamera()
{
    m_cameraEnabled = !m_cameraEnabled;
    m_controlBar->setCameraEnabled(m_cameraEnabled);

    if (m_localVideoWidget) {
        m_localVideoWidget->setVideoEnabled(m_cameraEnabled);
    }

    // TODO: Actually enable/disable camera
}

void MeetingRoomWindow::onToggleScreenShare()
{
    m_screenSharing = !m_screenSharing;
    m_controlBar->setScreenSharing(m_screenSharing);

    if (m_screenSharing) {
        emit startScreenShare();
    } else {
        emit stopScreenShare();
    }
}

void MeetingRoomWindow::onToggleChat()
{
    m_chatVisible = !m_chatVisible;
    
    if (m_chatVisible) {
        showChatPanel();
    } else {
        hideChatPanel();
    }
}

void MeetingRoomWindow::onToggleParticipants()
{
    m_participantsVisible = !m_participantsVisible;

    if (m_participantsVisible) {
        showParticipantsPanel();
    } else {
        hideParticipantsPanel();
    }
}

void MeetingRoomWindow::onLeaveMeeting()
{
    auto reply = QMessageBox::question(this, "离开会议", "确定要离开会议吗?",
                                       QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        m_durationTimer->stop();
        emit leaveMeeting();
    }
}

void MeetingRoomWindow::onMoreOptionsClicked()
{
    // TODO: Show more options menu
}

void MeetingRoomWindow::updateMeetingDuration()
{
    qint64 elapsed = m_meetingElapsed.elapsed();
    int seconds = (elapsed / 1000) % 60;
    int minutes = (elapsed / 60000) % 60;
    int hours = elapsed / 3600000;

    m_durationLabel->setText(QString("%1:%2:%3")
        .arg(hours, 2, 10, QChar('0'))
        .arg(minutes, 2, 10, QChar('0'))
        .arg(seconds, 2, 10, QChar('0')));
}

void MeetingRoomWindow::updateDurationDisplay()
{
    updateMeetingDuration();
}

void MeetingRoomWindow::showChatPanel()
{
    m_sidePanel->setFixedWidth(320);
    m_chatWidget->setVisible(true);
    m_participantWidget->setVisible(false);
}

void MeetingRoomWindow::hideChatPanel()
{
    if (!m_participantsVisible) {
        m_sidePanel->setFixedWidth(0);
    }
    m_chatWidget->setVisible(false);
}

void MeetingRoomWindow::showParticipantsPanel()
{
    m_sidePanel->setFixedWidth(280);
    m_participantWidget->setVisible(true);
    m_chatWidget->setVisible(false);
}

void MeetingRoomWindow::hideParticipantsPanel()
{
    if (!m_chatVisible) {
        m_sidePanel->setFixedWidth(0);
    }
    m_participantWidget->setVisible(false);
}
