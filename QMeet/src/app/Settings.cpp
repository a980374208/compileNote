#include "Settings.h"

Settings::Settings()
    : m_settings("QMeet", "QMeet")
{
}

Settings::~Settings()
{
    m_settings.sync();
}

QString Settings::lastUsername() const
{
    return m_settings.value("user/lastUsername").toString();
}

void Settings::setLastUsername(const QString &username)
{
    m_settings.setValue("user/lastUsername", username);
}

bool Settings::rememberMe() const
{
    return m_settings.value("user/rememberMe", true).toBool();
}

void Settings::setRememberMe(bool remember)
{
    m_settings.setValue("user/rememberMe", remember);
}

QString Settings::audioInputDevice() const
{
    return m_settings.value("audio/inputDevice").toString();
}

void Settings::setAudioInputDevice(const QString &deviceId)
{
    m_settings.setValue("audio/inputDevice", deviceId);
}

QString Settings::audioOutputDevice() const
{
    return m_settings.value("audio/outputDevice").toString();
}

void Settings::setAudioOutputDevice(const QString &deviceId)
{
    m_settings.setValue("audio/outputDevice", deviceId);
}

bool Settings::audioMuted() const
{
    return m_settings.value("audio/muted", false).toBool();
}

void Settings::setAudioMuted(bool muted)
{
    m_settings.setValue("audio/muted", muted);
}

int Settings::audioVolume() const
{
    return m_settings.value("audio/volume", 100).toInt();
}

void Settings::setAudioVolume(int volume)
{
    m_settings.setValue("audio/volume", volume);
}

QString Settings::videoInputDevice() const
{
    return m_settings.value("video/inputDevice").toString();
}

void Settings::setVideoInputDevice(const QString &deviceId)
{
    m_settings.setValue("video/inputDevice", deviceId);
}

bool Settings::videoEnabled() const
{
    return m_settings.value("video/enabled", true).toBool();
}

void Settings::setVideoEnabled(bool enabled)
{
    m_settings.setValue("video/enabled", enabled);
}

QString Settings::videoResolution() const
{
    return m_settings.value("video/resolution", "1280x720").toString();
}

void Settings::setVideoResolution(const QString &resolution)
{
    m_settings.setValue("video/resolution", resolution);
}

bool Settings::autoJoinAudio() const
{
    return m_settings.value("meeting/autoJoinAudio", true).toBool();
}

void Settings::setAutoJoinAudio(bool enabled)
{
    m_settings.setValue("meeting/autoJoinAudio", enabled);
}

bool Settings::autoEnableVideo() const
{
    return m_settings.value("meeting/autoEnableVideo", true).toBool();
}

void Settings::setAutoEnableVideo(bool enabled)
{
    m_settings.setValue("meeting/autoEnableVideo", enabled);
}

bool Settings::showNotifications() const
{
    return m_settings.value("meeting/showNotifications", true).toBool();
}

void Settings::setShowNotifications(bool enabled)
{
    m_settings.setValue("meeting/showNotifications", enabled);
}

QString Settings::serverUrl() const
{
    return m_settings.value("network/serverUrl", "wss://meet.example.com").toString();
}

void Settings::setServerUrl(const QString &url)
{
    m_settings.setValue("network/serverUrl", url);
}

int Settings::networkTimeout() const
{
    return m_settings.value("network/timeout", 30000).toInt();
}

void Settings::setNetworkTimeout(int timeout)
{
    m_settings.setValue("network/timeout", timeout);
}

QString Settings::language() const
{
    return m_settings.value("general/language", "zh_CN").toString();
}

void Settings::setLanguage(const QString &lang)
{
    m_settings.setValue("general/language", lang);
}

QString Settings::theme() const
{
    return m_settings.value("general/theme", "light").toString();
}

void Settings::setTheme(const QString &theme)
{
    m_settings.setValue("general/theme", theme);
}
