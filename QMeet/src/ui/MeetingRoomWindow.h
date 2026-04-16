#ifndef MEETINGROOMWINDOW_H
#define MEETINGROOMWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QElapsedTimer>
#include "../models/Meeting.h"
#include "../models/User.h"

class VideoWidget;
class ControlBar;
class ChatWidget;
class ParticipantWidget;
class ScreenShareWidget;
class ScreenShareManager;
class MeetingManager;

/**
 * @brief 会议室窗口
 * 显示视频、控制栏、参与者列表和聊天
 */
class MeetingRoomWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MeetingRoomWindow(QWidget *parent = nullptr);
    ~MeetingRoomWindow() override;

    void joinMeeting(const QString &meetingId);
    void setMeeting(Meeting *meeting);

signals:
    void leaveMeeting();
    void startScreenShare();
    void stopScreenShare();

private slots:
    void onToggleMicrophone();
    void onToggleCamera();
    void onToggleScreenShare();
    void onToggleChat();
    void onToggleParticipants();
    void onLeaveMeeting();
    void onMoreOptionsClicked();
    void updateMeetingDuration();

private:
    void setupUI();
    void setupConnections();
    void initializeMediaDevices();
    void updateDurationDisplay();
    void showChatPanel();
    void hideChatPanel();
    void showParticipantsPanel();
    void hideParticipantsPanel();

    Meeting *m_meeting = nullptr;
    MeetingManager *m_meetingManager = nullptr;

    // State
    bool m_microphoneEnabled = true;
    bool m_cameraEnabled = true;
    bool m_screenSharing = false;
    bool m_chatVisible = false;
    bool m_participantsVisible = false;

    // Timer for meeting duration
    QTimer *m_durationTimer = nullptr;
    QElapsedTimer m_meetingElapsed;

    // Header widgets
    QLabel *m_meetingTitleLabel = nullptr;
    QLabel *m_meetingNumberLabel = nullptr;
    QLabel *m_durationLabel = nullptr;
    QPushButton *m_moreBtn = nullptr;

    // Video area
    QWidget *m_videoArea = nullptr;
    VideoWidget *m_localVideoWidget = nullptr;
    QList<VideoWidget*> m_remoteVideoWidgets;

    // Control bar
    ControlBar *m_controlBar = nullptr;

    // Side panels
    QWidget *m_sidePanel = nullptr;
    ChatWidget *m_chatWidget = nullptr;
    ParticipantWidget *m_participantWidget = nullptr;

    // Screen share
    ScreenShareWidget *m_screenShareWidget = nullptr;
    ScreenShareManager *m_screenShareManager = nullptr;
};

#endif // MEETINGROOMWINDOW_H
