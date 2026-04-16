#include "ScreenShareWidget.h"
#include "TransparentDrawingWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPainter>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QPixmap>
#include <QColor>
#include <QFont>
#include <QPropertyAnimation>

ScreenShareWidget::ScreenShareWidget(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
}

void ScreenShareWidget::setScreenImage(const QImage &image)
{
    m_currentImage = image;
    update();  // Trigger repaint
}

void ScreenShareWidget::setSharerName(const QString &name)
{
    m_sharerName = name;
    if (m_sharerLabel) {
        m_sharerLabel->setText(QString("%1 正在共享屏幕").arg(name));
    }
}

void ScreenShareWidget::setSharing(bool sharing)
{
    m_isSharing = sharing;
    m_overlay->setVisible(sharing);
    if (m_drawingBtn) {
        m_drawingBtn->setVisible(sharing);
    }
}

void ScreenShareWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    painter.fillRect(rect(), QColor("#0a0a0a"));

    if (m_currentImage.isNull()) {
        painter.setPen(QColor("#666666"));
        painter.setFont(QFont("Arial", 16));
        painter.drawText(rect(), Qt::AlignCenter, "等待屏幕共享...");
        return;
    }

    QRect imageRect = calculateImageRect();
    painter.drawImage(imageRect, m_currentImage);
}

void ScreenShareWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    // Update overlay to span the top of the widget
    if (m_overlay) {
        m_overlay->setGeometry(0, 0, width(), 50);
    }
    
    // Position drawing button at bottom right
    if (m_drawingBtn) {
        int btnSize = 56;
        m_drawingBtn->setGeometry(width() - btnSize - 20, height() - btnSize - 20, btnSize, btnSize);
    }
}

QRect ScreenShareWidget::calculateImageRect() const
{
    if (m_currentImage.isNull()) {
        return rect();
    }
    
    QSize widgetSize = size();
    QSize imageSize = m_currentImage.size();
    
    // Calculate scale factor to fill the widget while keeping aspect ratio
    QSize targetSize = imageSize.scaled(widgetSize, Qt::KeepAspectRatio);
    int x = (widgetSize.width() - targetSize.width()) / 2;
    int y = (widgetSize.height() - targetSize.height()) / 2;
    return QRect(x, y, targetSize.width(), targetSize.height());
}

void ScreenShareWidget::setupUI()
{
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setStyleSheet(R"(
        QWidget {
            background-color: #0a0a0a;
        }
    )");

    // Overlay (shown when sharing)
    m_overlay = new QWidget(this);
    m_overlay->setStyleSheet("background: transparent;");
    QHBoxLayout *overlayLayout = new QHBoxLayout(m_overlay);
    overlayLayout->setContentsMargins(16, 16, 16, 16);

    m_sharerLabel = new QLabel("正在共享屏幕", this);
    m_sharerLabel->setStyleSheet(R"(
        background-color: rgba(0, 0, 0, 0.7);
        color: white;
        border-radius: 4px;
        padding: 4px 12px;
        font-size: 12px;
    )");

    m_stopBtn = new QPushButton("停止共享", this);
    m_stopBtn->setStyleSheet(R"(
    QPushButton {
        background-color: #d13438;
        color: white;
        border: none;
        border-radius: 4px;
        padding: 4px 12px;
        font-size: 12px;
    }
    QPushButton:hover {
        background-color: #b22b2e;
    }
    )");
    connect(m_stopBtn, &QPushButton::clicked, this, &ScreenShareWidget::stopSharing);

    overlayLayout->addWidget(m_sharerLabel);
    overlayLayout->addStretch();
    overlayLayout->addWidget(m_stopBtn);

    // Position overlay at top
    m_overlay->setGeometry(0, 0, width(), 50);
    m_overlay->setVisible(false);
    
    // Setup drawing button
    setupDrawingButton();
}

void ScreenShareWidget::setupDrawingButton()
{
    m_drawingBtn = new QPushButton(this);
    m_drawingBtn->setFixedSize(56, 56);
    m_drawingBtn->setText("✏️");
    m_drawingBtn->setStyleSheet(R"(
        QPushButton {
            background: qradialgradient(cx:0.5, cy:0.5, radius:30, stop:0 #4caf50, stop:1 #2e7d32);
            border: 3px solid #64b5f6;
            border-radius: 28px;
            color: white;
            font-size: 24px;
            font-weight: bold;
            padding: 0px;
            outline: none;
        }
        QPushButton:hover {
            background: qradialgradient(cx:0.5, cy:0.5, radius:30, stop:0 #66bb6a, stop:1 #388e3c);
            border: 3px solid #42a5f5;
        }
        QPushButton:pressed {
            background: qradialgradient(cx:0.5, cy:0.5, radius:30, stop:0 #43a047, stop:1 #1b5e20);
            border: 3px solid #1e88e5;
        }
    )");
    m_drawingBtn->setCursor(Qt::PointingHandCursor);
    m_drawingBtn->setVisible(false);
    
    connect(m_drawingBtn, &QPushButton::clicked, this, &ScreenShareWidget::onDrawingButtonClicked);
}

void ScreenShareWidget::onDrawingButtonClicked()
{
    if (!m_drawingWindow) {
        m_drawingWindow = new TransparentDrawingWindow();
        connect(m_drawingWindow, &TransparentDrawingWindow::drawingFinished, 
                this, [this]() {
                    m_drawingWindow = nullptr;
                });
    }
    
    m_drawingWindow->show();
    m_drawingWindow->raise();  // Ensure window stays on top
    m_drawingWindow->setFocus();
}
