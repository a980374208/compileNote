#include "VideoWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPainter>
#include <QPainterPath>
#include <QBrush>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QFont>

VideoWidget::VideoWidget(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
}

VideoWidget::~VideoWidget()
{
}

void VideoWidget::setVideoSink(QVideoSink *sink)
{
    if (m_videoWidget && sink) {
        // m_videoWidget->setVideoSink(sink);
    }
}

void VideoWidget::setIsLocal(bool isLocal)
{
    m_isLocal = isLocal;
    updateAvatarDisplay();
}

void VideoWidget::setParticipantName(const QString &name)
{
    m_participantName = name;
    if (m_nameLabel) {
        m_nameLabel->setText(name);
    }
    if (m_avatarLabel && m_avatar.isNull()) {
        // Show first character as avatar
        if (!name.isEmpty()) {
            m_avatarLabel->setText(name.at(0).toUpper());
        }
    }
}

void VideoWidget::setMuted(bool muted)
{
    m_muted = muted;
    if (m_muteIndicator) {
        m_muteIndicator->setVisible(muted);
    }
}

void VideoWidget::setVideoEnabled(bool enabled)
{
    m_videoEnabled = enabled;
    updateAvatarDisplay();
}

void VideoWidget::setAvatar(const QPixmap &avatar)
{
    m_avatar = avatar;
    updateAvatarDisplay();
}

void VideoWidget::setupUI()
{
    setStyleSheet(R"(
        VideoWidget {
            background-color: #1a1a1a;
            border-radius: 8px;
        }
    )");
    setMinimumSize(320, 240);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // Video container
    m_videoContainer = new QWidget();
    m_videoContainer->setStyleSheet("background-color: #0a0a0a;");
    QVBoxLayout *containerLayout = new QVBoxLayout(m_videoContainer);
    containerLayout->setContentsMargins(0, 0, 0, 0);

    // Video widget for actual video
    m_videoWidget = new QVideoWidget();
    m_videoWidget->setStyleSheet("background-color: transparent;");
    m_videoWidget->setVisible(false);
    containerLayout->addWidget(m_videoWidget);

    // Avatar label (shown when video is off)
    m_avatarLabel = new QLabel();
    m_avatarLabel->setAlignment(Qt::AlignCenter);
    m_avatarLabel->setStyleSheet(R"(
        background-color: #0078d4;
        border-radius: 40px;
        color: white;
        font-size: 32px;
        font-weight: bold;
    )");
    m_avatarLabel->setFixedSize(80, 80);
    m_avatarLabel->setText("U");
    containerLayout->addWidget(m_avatarLabel, 0, Qt::AlignCenter);

    mainLayout->addWidget(m_videoContainer, 1);

    // Overlay for name and status
    QWidget *overlay = new QWidget();
    overlay->setStyleSheet("background: transparent;");
    QHBoxLayout *overlayLayout = new QHBoxLayout(overlay);
    overlayLayout->setContentsMargins(8, 0, 8, 8);

    // Name label
    m_nameLabel = new QLabel("用户名");
    m_nameLabel->setStyleSheet(R"(
        background-color: rgba(0, 0, 0, 0.6);
        border-radius: 4px;
        padding: 4px 8px;
        color: white;
        font-size: 12px;
    )");

    // Mute indicator
    m_muteIndicator = new QLabel("🔇");
    m_muteIndicator->setStyleSheet(R"(
        background-color: rgba(0, 0, 0, 0.6);
        border-radius: 4px;
        padding: 4px 8px;
        font-size: 12px;
    )");
    m_muteIndicator->setVisible(false);

    overlayLayout->addWidget(m_nameLabel);
    overlayLayout->addStretch();
    overlayLayout->addWidget(m_muteIndicator);

    mainLayout->addWidget(overlay, 0, Qt::AlignBottom);
}

void VideoWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Draw background
    painter.fillRect(rect(), QColor("#1a1a1a"));

    // Draw rounded corners
    QPainterPath path;
    path.addRoundedRect(rect(), 8, 8);
    painter.setClipPath(path);
}

void VideoWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
}

void VideoWidget::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    emit clicked();
}

void VideoWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    emit doubleClicked();
}

void VideoWidget::updateAvatarDisplay()
{
    if (m_videoEnabled && m_videoWidget) {
        m_videoWidget->setVisible(true);
        m_avatarLabel->setVisible(false);
    } else {
        m_videoWidget->setVisible(false);
        m_avatarLabel->setVisible(true);
    }
}
