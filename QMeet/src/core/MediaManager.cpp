#include "MediaManager.h"
#include <QMediaDevices>
#include <QAudioDevice>
#include <QCameraDevice>

MediaManager::MediaManager(QObject *parent)
    : QObject(parent)
    , m_videoSink(new QVideoSink(this))
{
    initializeDevices();
}

MediaManager::~MediaManager()
{
    if (m_camera) {
        m_camera->stop();
    }
    delete m_captureSession;
}

QList<QString> MediaManager::availableCameras() const
{
    QList<QString> cameras;
    const QList<QCameraDevice> devices = QMediaDevices::videoInputs();
    for (const QCameraDevice &device : devices) {
        cameras.append(device.description());
    }
    return cameras;
}

QList<QString> MediaManager::availableAudioInputs() const
{
    QList<QString> inputs;
    const QList<QAudioDevice> devices = QMediaDevices::audioInputs();
    for (const QAudioDevice &device : devices) {
        inputs.append(device.description());
    }
    return inputs;
}

QList<QString> MediaManager::availableAudioOutputs() const
{
    QList<QString> outputs;
    const QList<QAudioDevice> devices = QMediaDevices::audioOutputs();
    for (const QAudioDevice &device : devices) {
        outputs.append(device.description());
    }
    return outputs;
}

void MediaManager::setCamera(const QString &deviceName)
{
    Q_UNUSED(deviceName);
    // TODO: Implement camera selection
}

void MediaManager::setAudioInput(const QString &deviceName)
{
    Q_UNUSED(deviceName);
    // TODO: Implement audio input selection
}

void MediaManager::setAudioOutput(const QString &deviceName)
{
    Q_UNUSED(deviceName);
    // TODO: Implement audio output selection
}

bool MediaManager::isCameraEnabled() const
{
    return m_cameraEnabled;
}

bool MediaManager::isMicrophoneEnabled() const
{
    return m_microphoneEnabled;
}

bool MediaManager::isSpeakerEnabled() const
{
    return m_speakerEnabled;
}

void MediaManager::setCameraEnabled(bool enabled)
{
    if (m_cameraEnabled != enabled) {
        m_cameraEnabled = enabled;
        if (m_camera) {
            if (enabled) {
                m_camera->start();
            } else {
                m_camera->stop();
            }
        }
        emit cameraEnabledChanged(enabled);
    }
}

void MediaManager::setMicrophoneEnabled(bool enabled)
{
    if (m_microphoneEnabled != enabled) {
        m_microphoneEnabled = enabled;
        if (m_audioInput) {
            m_audioInput->setMuted(!enabled);
        }
        emit microphoneEnabledChanged(enabled);
    }
}

void MediaManager::setSpeakerEnabled(bool enabled)
{
    if (m_speakerEnabled != enabled) {
        m_speakerEnabled = enabled;
        emit speakerEnabledChanged(enabled);
    }
}

QVideoSink* MediaManager::videoSink() const
{
    return m_videoSink;
}

QImage MediaManager::captureFrame()
{
    // TODO: Implement frame capture
    return QImage();
}

int MediaManager::microphoneVolume() const
{
    return m_microphoneVolume;
}

int MediaManager::speakerVolume() const
{
    return m_speakerVolume;
}

void MediaManager::setMicrophoneVolume(int volume)
{
    m_microphoneVolume = qBound(0, volume, 100);
    if (m_audioInput) {
        m_audioInput->setVolume(m_microphoneVolume / 100.0);
    }
}

void MediaManager::setSpeakerVolume(int volume)
{
    m_speakerVolume = qBound(0, volume, 100);
    if (m_audioOutput) {
        m_audioOutput->setVolume(m_speakerVolume / 100.0);
    }
}

void MediaManager::initializeDevices()
{
    // Initialize capture session
    m_captureSession = new QMediaCaptureSession(this);

    // Use default camera
    const QList<QCameraDevice> cameras = QMediaDevices::videoInputs();
    if (!cameras.isEmpty()) {
        m_camera = new QCamera(cameras.first(), this);
        m_captureSession->setCamera(m_camera);
        m_camera->start();
    }

    // Set video output
    m_captureSession->setVideoOutput(m_videoSink);

    // Use default audio input
    const QList<QAudioDevice> audioInputs = QMediaDevices::audioInputs();
    if (!audioInputs.isEmpty()) {
        m_audioInput = new QAudioInput(audioInputs.first(), this);
        m_captureSession->setAudioInput(m_audioInput);
    }

    // Use default audio output
    const QList<QAudioDevice> audioOutputs = QMediaDevices::audioOutputs();
    if (!audioOutputs.isEmpty()) {
        m_audioOutput = new QAudioOutput(audioOutputs.first(), this);
    }
}

void MediaManager::setupAudioMonitoring()
{
    // TODO: Setup audio level monitoring
}
