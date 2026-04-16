#ifndef SCREENSHAREMANAGER_H
#define SCREENSHAREMANAGER_H

#include <QObject>
#include <QImage>
#include <QScreen>
#include <QTimer>

/**
 * @brief 屏幕共享管理器
 * 负责屏幕捕获和共享功能
 */
class ScreenShareManager : public QObject
{
    Q_OBJECT

public:
    explicit ScreenShareManager(QObject *parent = nullptr);
    ~ScreenShareManager() override;

    QList<QString> availableScreens() const;
    QList<QString> availableWindows() const;

    void startScreenShare(int screenIndex = -1);  // -1 for entire screen
    void startWindowShare(int windowIndex);
    void stopShare();

    bool isSharing() const;
    QString sharerName() const;

    void setCaptureInterval(int ms);  // default 50ms (20fps)

signals:
    void shareStarted();
    void shareStopped();
    void frameReady(const QImage &frame);
    void error(const QString &message);

private slots:
    void captureFrame();

private:
    QScreen *m_targetScreen = nullptr;
    QTimer *m_captureTimer = nullptr;
    bool m_isSharing = false;
    QString m_sharerName;
    int m_captureInterval = 50;
};

#endif // SCREENSHAREMANAGER_H
