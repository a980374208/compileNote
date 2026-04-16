#ifndef VIDEOWIDGET_H
#define VIDEOWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QVideoSink>
#include <QMediaPlayer>
#include <QVideoWidget>

/**
 * @brief 视频显示组件
 * 显示本地或远程参与者的视频流
 */
class VideoWidget : public QWidget
{
    Q_OBJECT

public:
    explicit VideoWidget(QWidget *parent = nullptr);
    ~VideoWidget() override;

    void setVideoSink(QVideoSink *sink);
    void setIsLocal(bool isLocal);
    void setParticipantName(const QString &name);
    void setMuted(bool muted);
    void setVideoEnabled(bool enabled);
    void setAvatar(const QPixmap &avatar);

    bool isLocal() const { return m_isLocal; }
    bool isMuted() const { return m_muted; }
    bool isVideoEnabled() const { return m_videoEnabled; }

signals:
    void clicked();
    void doubleClicked();

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;

private:
    void setupUI();
    void updateAvatarDisplay();

    bool m_isLocal = false;
    bool m_muted = false;
    bool m_videoEnabled = true;
    QString m_participantName;
    QPixmap m_avatar;

    // UI Components
    QWidget *m_videoContainer = nullptr;
    QVideoWidget *m_videoWidget = nullptr;
    QLabel *m_avatarLabel = nullptr;
    QLabel *m_nameLabel = nullptr;
    QLabel *m_muteIndicator = nullptr;
};

#endif // VIDEOWIDGET_H
