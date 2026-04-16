#ifndef CONTROLBAR_H
#define CONTROLBAR_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>

/**
 * @brief 会议控制栏
 * 提供麦克风、摄像头、屏幕共享等控制按钮
 */
class ControlBar : public QWidget
{
    Q_OBJECT

public:
    explicit ControlBar(QWidget *parent = nullptr);
    ~ControlBar() override = default;

    void setMicrophoneEnabled(bool enabled);
    void setCameraEnabled(bool enabled);
    void setScreenSharing(bool sharing);
    void setChatVisible(bool visible);
    void setParticipantsVisible(bool visible);

    bool microphoneEnabled() const { return m_microphoneEnabled; }
    bool cameraEnabled() const { return m_cameraEnabled; }
    bool screenSharing() const { return m_screenSharing; }

signals:
    void toggleMicrophone();
    void toggleCamera();
    void toggleScreenShare();
    void toggleChat();
    void toggleParticipants();
    void leaveMeeting();
    void settingsClicked();

private slots:
    void onMicClicked();
    void onCameraClicked();
    void onScreenShareClicked();
    void onChatClicked();
    void onParticipantsClicked();
    void onLeaveClicked();

private:
    void setupUI();
    void setupConnections();
    QPushButton* createControlButton(const QString &icon, const QString &tooltip);
    void updateButtonStyle(QPushButton *button, bool active, const QString &activeColor = "#d13438");

    // State
    bool m_microphoneEnabled = true;
    bool m_cameraEnabled = true;
    bool m_screenSharing = false;
    bool m_chatVisible = false;
    bool m_participantsVisible = false;

    // Buttons
    QPushButton *m_micBtn = nullptr;
    QPushButton *m_cameraBtn = nullptr;
    QPushButton *m_screenShareBtn = nullptr;
    QPushButton *m_chatBtn = nullptr;
    QPushButton *m_participantsBtn = nullptr;
    QPushButton *m_leaveBtn = nullptr;
};

#endif // CONTROLBAR_H
