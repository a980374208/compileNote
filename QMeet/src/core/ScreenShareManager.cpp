#include "ScreenShareManager.h"
#include <QGuiApplication>
#include <QWindow>
#include <QApplication>
#include <QDesktopServices>
#include <QUrl>

#ifdef Q_OS_MAC
#include <CoreGraphics/CoreGraphics.h>
#endif

ScreenShareManager::ScreenShareManager(QObject *parent)
    : QObject(parent)
    , m_captureTimer(new QTimer(this))
{
    connect(m_captureTimer, &QTimer::timeout, this, &ScreenShareManager::captureFrame);
}

ScreenShareManager::~ScreenShareManager()
{
    stopShare();
}

QList<QString> ScreenShareManager::availableScreens() const
{
    QList<QString> screens;
    const QList<QScreen *> screenList = QGuiApplication::screens();
    for (QScreen *screen : screenList) {
        screens.append(screen->name());
    }
    return screens;
}

QList<QString> ScreenShareManager::availableWindows() const
{
    QList<QString> windows;
    // TODO: Implement window enumeration
    return windows;
}

void ScreenShareManager::startScreenShare(int screenIndex)
{
    const QList<QScreen *> screens = QGuiApplication::screens();
    
    if (screenIndex < 0 || screenIndex >= screens.size()) {
        // Use primary screen
        m_targetScreen = QGuiApplication::primaryScreen();
    } else {
        m_targetScreen = screens[screenIndex];
    }

    if (!m_targetScreen) {
        emit error("没有可用的屏幕进行共享");
        return;
    }

    m_isSharing = true;
    m_sharerName = "我";
    m_captureTimer->start(m_captureInterval);
    
    emit shareStarted();
}

void ScreenShareManager::startWindowShare(int windowIndex)
{
    Q_UNUSED(windowIndex);
    // TODO: Implement window sharing
    emit error("窗口共享功能尚未实现");
}

void ScreenShareManager::stopShare()
{
    if (m_isSharing) {
        m_captureTimer->stop();
        m_isSharing = false;
        m_targetScreen = nullptr;
        emit shareStopped();
    }
}

bool ScreenShareManager::isSharing() const
{
    return m_isSharing;
}

QString ScreenShareManager::sharerName() const
{
    return m_sharerName;
}

void ScreenShareManager::setCaptureInterval(int ms)
{
    m_captureInterval = ms;
    if (m_captureTimer->isActive()) {
        m_captureTimer->setInterval(m_captureInterval);
    }
}

void ScreenShareManager::captureFrame()
{
    if (!m_targetScreen || !m_isSharing) {
        return;
    }

    // grabWindow(0) captures the entire screen
    // On macOS, this requires Screen Recording permission in System Preferences > Security & Privacy > Privacy > Screen Recording
    QPixmap pixmap = m_targetScreen->grabWindow(0);
    
    if (pixmap.isNull()) {
        return;
    }
    
    // Check if captured image is mostly black (indicates permission issue on macOS)
    // This happens when screen recording permission is not granted
    QImage img = pixmap.toImage();
    bool hasContent = false;
    
    // Sample some pixels to check if we got actual content
    int sampleStep = qMax(1, img.width() / 20);
    for (int y = 0; y < img.height() && !hasContent; y += sampleStep) {
        for (int x = 0; x < img.width() && !hasContent; x += sampleStep) {
            QRgb pixel = img.pixel(x, y);
            // Check if pixel is not black (has some color)
            int r = qRed(pixel);
            int g = qGreen(pixel);
            int b = qBlue(pixel);
            if (r > 10 || g > 10 || b > 10) {
                hasContent = true;
            }
        }
    }
    
    static bool permissionWarned = false;
    if (!hasContent && !permissionWarned) {
        permissionWarned = true;
        emit error("屏幕内容为空，可能缺少屏幕录制权限。\n\n请在「系统偏好设置 > 安全性与隐私 > 隐私 > 屏幕录制」中授权此应用，然后重启应用。");
        stopShare();
        return;
    }
    
    emit frameReady(img);
}
