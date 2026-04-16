#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>
#include <QSettings>
#include <QVariant>

/**
 * @brief 应用程序设置管理类
 * 使用 QSettings 持久化存储用户设置
 */
class Settings
{
public:
    Settings();
    ~Settings();

    // User settings
    QString lastUsername() const;
    void setLastUsername(const QString &username);
    bool rememberMe() const;
    void setRememberMe(bool remember);

    // Audio settings
    QString audioInputDevice() const;
    void setAudioInputDevice(const QString &deviceId);
    QString audioOutputDevice() const;
    void setAudioOutputDevice(const QString &deviceId);
    bool audioMuted() const;
    void setAudioMuted(bool muted);
    int audioVolume() const;
    void setAudioVolume(int volume);

    // Video settings
    QString videoInputDevice() const;
    void setVideoInputDevice(const QString &deviceId);
    bool videoEnabled() const;
    void setVideoEnabled(bool enabled);
    QString videoResolution() const;
    void setVideoResolution(const QString &resolution);

    // Meeting settings
    bool autoJoinAudio() const;
    void setAutoJoinAudio(bool enabled);
    bool autoEnableVideo() const;
    void setAutoEnableVideo(bool enabled);
    bool showNotifications() const;
    void setShowNotifications(bool enabled);

    // Network settings
    QString serverUrl() const;
    void setServerUrl(const QString &url);
    int networkTimeout() const;
    void setNetworkTimeout(int timeout);

    // Language
    QString language() const;
    void setLanguage(const QString &lang);

    // Theme
    QString theme() const;
    void setTheme(const QString &theme);

private:
    QSettings m_settings;
};

#endif // SETTINGS_H
