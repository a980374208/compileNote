#ifndef MEDIAMANAGER_H
#define MEDIAMANAGER_H

#include <QObject>
#include <QCamera>
#include <QAudioInput>
#include <QAudioOutput>
#include <QMediaCaptureSession>
#include <QVideoSink>
#include <QImage>
#include <QList>

/**
 * @brief 媒体管理器
 * 负责摄像头、麦克风等媒体设备的管理
 */
class MediaManager : public QObject
{
    Q_OBJECT

public:
    explicit MediaManager(QObject *parent = nullptr);
    ~MediaManager() override;

    // Device enumeration
    QList<QString> availableCameras() const;
    QList<QString> availableAudioInputs() const;
    QList<QString> availableAudioOutputs() const;

    // Device selection
    void setCamera(const QString &deviceName);
    void setAudioInput(const QString &deviceName);
    void setAudioOutput(const QString &deviceName);

    // State management
    bool isCameraEnabled() const;
    bool isMicrophoneEnabled() const;
    bool isSpeakerEnabled() const;

    void setCameraEnabled(bool enabled);
    void setMicrophoneEnabled(bool enabled);
    void setSpeakerEnabled(bool enabled);

    // Video capture
    QVideoSink* videoSink() const;
    QImage captureFrame();

    // Audio settings
    int microphoneVolume() const;
    int speakerVolume() const;
    void setMicrophoneVolume(int volume);  // 0-100
    void setSpeakerVolume(int volume);     // 0-100

signals:
    void cameraEnabledChanged(bool enabled);
    void microphoneEnabledChanged(bool enabled);
    void speakerEnabledChanged(bool enabled);
    void cameraFrameReady(const QImage &frame);
    void audioLevelChanged(float level);
    void error(const QString &message);

private:
    void initializeDevices();
    void setupAudioMonitoring();

    QCamera *m_camera = nullptr;
    QAudioInput *m_audioInput = nullptr;
    QAudioOutput *m_audioOutput = nullptr;
    QMediaCaptureSession *m_captureSession = nullptr;
    QVideoSink *m_videoSink = nullptr;

    bool m_cameraEnabled = true;
    bool m_microphoneEnabled = true;
    bool m_speakerEnabled = true;

    int m_microphoneVolume = 100;
    int m_speakerVolume = 100;
};

#endif // MEDIAMANAGER_H
