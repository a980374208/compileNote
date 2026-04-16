#include "TransparentDrawingWindow.h"
#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QShowEvent>
#include <QApplication>
#include <QScreen>

// macOS-specific function declarations
#ifdef Q_OS_MACOS
extern "C" void setupMouseEventPassthrough(void *winId, bool enable);
extern "C" void bringWindowToFront(void *winId);
extern "C" bool isWindowInFront(void *winId);
extern "C" void bringWindowToFrontIfNeeded(void *winId);
#endif

TransparentDrawingWindow::TransparentDrawingWindow(QWidget *parent)
    : QWidget(parent), m_focusPassthrough(false)
{
    // Set window properties for transparent overlay
    // WindowStaysOnTopHint ensures the window stays on top during its lifetime
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_NoSystemBackground);
    
    // By default, do NOT allow mouse events to pass through
    // Events pass through via event->ignore() mechanism, not via Qt::WA_TransparentForMouseEvents
    
    // Set focus policy to not accept keyboard focus
    setFocusPolicy(Qt::NoFocus);
    updateCursor();  // Set initial cursor based on focus pass-through state
    
    // Cover entire screen
    QScreen *screen = QApplication::primaryScreen();
    if (screen) {
        setGeometry(screen->geometry());
    }
    
    // Initialize window order monitoring to keep this window always on top
    //initWindowOrderMonitoring();
}

void TransparentDrawingWindow::setFocusPassthrough(bool enable)
{
    if (m_focusPassthrough != enable) {
        m_focusPassthrough = enable;
        enableDrawing(!m_focusPassthrough);
        // Note: We don't use Qt::WA_TransparentForMouseEvents on macOS
        // Instead, we use event->ignore() to pass events through the event chain
        enableMouseEventPassthrough(enable);
        updateCursor();
    }
}

void TransparentDrawingWindow::enableMouseEventPassthrough(bool enable)
{
#ifdef Q_OS_MACOS
    // Call the macOS-specific implementation
    setupMouseEventPassthrough(reinterpret_cast<void *>(winId()), enable);
#else
    Q_UNUSED(enable);
#endif
}

void TransparentDrawingWindow::bringToFront()
{
#ifdef Q_OS_MACOS
    // Call the macOS-specific implementation to bring window to front
    bringWindowToFront(reinterpret_cast<void *>(winId()));
#else
    // On other platforms, use Qt's raise() method
    raise();
    activateWindow();
#endif
}

void TransparentDrawingWindow::updateCursor()
{
    // When focus pass-through is enabled, use arrow cursor
    // When disabled, use pointing hand cursor
    if (m_focusPassthrough) {
        setCursor(Qt::ArrowCursor);
    } else {
        setCursor(Qt::PointingHandCursor);
    }
}

void TransparentDrawingWindow::clearDrawings()
{
    m_drawings.clear();
    m_currentStroke.clear();
    m_isDrawing = false;
    update();
}

void TransparentDrawingWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    // Draw semi-transparent overlay
    painter.fillRect(rect(), QColor(0, 0, 0, 10));
    
    // Draw all completed strokes
    if (!m_drawings.isEmpty()) {
        painter.setPen(QPen(m_drawingColor, m_drawingLineWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        for (const auto &stroke : m_drawings) {
            if (stroke.size() > 1) {
                for (int i = 1; i < stroke.size(); ++i) {
                    painter.drawLine(stroke[i-1], stroke[i]);
                }
            }
        }
    }
    
    // Draw current stroke being drawn
    if (m_isDrawing && m_currentStroke.size() > 1) {
        painter.setPen(QPen(m_drawingColor, m_drawingLineWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        for (int i = 1; i < m_currentStroke.size(); ++i) {
            painter.drawLine(m_currentStroke[i-1], m_currentStroke[i]);
        }
    }
}

// void TransparentDrawingWindow::updateMouseTransparency()
// {
//     // When drawing, disable transparent-for-mouse to capture events
//     // When not drawing, enable it to let events pass through
//     bool shouldBeTransparent = !m_isDrawing;
//     setAttribute(Qt::WA_TransparentForMouseEvents, shouldBeTransparent);
// }

void TransparentDrawingWindow::mousePressEvent(QMouseEvent *event)
{
    // Right button click: toggle focus pass-through
    if (event->button() == Qt::RightButton) {
        setFocusPassthrough(!m_focusPassthrough);
        // Don't consume right-click, pass it through
        event->ignore();
        return;
    }
    
    // Left button: handle drawing or pass-through
    if (event->button() == Qt::LeftButton) {
        if (!m_drawingEnabled) {
            // Drawing disabled: pass through to lower windows
            event->ignore();
            return;
        }
        
        // Drawing enabled: start drawing
        m_isDrawing = true;
        m_currentStroke.clear();
        m_currentStroke.append(event->pos());
        update();
        
        event->accept();
        return;
    }
    
    event->ignore();
}

void TransparentDrawingWindow::mouseMoveEvent(QMouseEvent *event)
{
    // Only handle move events while actively drawing
    if (!m_isDrawing || !m_drawingEnabled) {
        event->ignore();
        return;
    }
    
    m_currentStroke.append(event->pos());
    update();
    
    event->accept();
}

void TransparentDrawingWindow::mouseReleaseEvent(QMouseEvent *event)
{
    // Only handle left button release for drawing
    if (event->button() != Qt::LeftButton) {
        event->ignore();
        return;
    }
    
    // If we're drawing, complete the stroke
    if (m_isDrawing) {
        m_currentStroke.append(event->pos());
        m_isDrawing = false;
        
        if (m_currentStroke.size() > 1) {
            m_drawings.append(m_currentStroke);
        }
        m_currentStroke.clear();
        update();
        
        event->accept();
        return;
    }
    
    // If not drawing, pass event through to lower windows
    event->ignore();
}

void TransparentDrawingWindow::keyPressEvent(QKeyEvent *event)
{
    // ESC key to close drawing window
    if (event->key() == Qt::Key_Escape) {
        close();
        emit drawingFinished();
        event->accept();
    } else {
        event->ignore();  // // Pass other keys to lower windows
    }
}

void TransparentDrawingWindow::showEvent(QShowEvent *event)
{
    // Call the base class implementation
    QWidget::showEvent(event);
    
    // Bring window to front when shown
    bringToFront();
}

void TransparentDrawingWindow::initWindowOrderMonitoring()
{
    // Create a timer to periodically check if window is still on top
    m_windowOrderTimer = new QTimer(this);
    connect(m_windowOrderTimer, &QTimer::timeout, this, &TransparentDrawingWindow::checkAndRestoreWindowOrder);
    
    // Check every 1000ms (1 second) if window is still in front
    // Using a longer interval to avoid excessive checks
    m_windowOrderTimer->start(1000);
}

void TransparentDrawingWindow::checkAndRestoreWindowOrder()
{
    // Only restore window to front if it's actually covered by another window
    // This avoids flashing by only acting when necessary
    if (isVisible()) {
#ifdef Q_OS_MACOS
        // Skip restoration if we detect that other windows are being dragged
        // This avoids constant flashing when users drag windows
        if (m_otherWindowBeingDragged) {
            // Restart the drag detection timer
            if (!m_dragDetectTimer) {
                m_dragDetectTimer = new QTimer(this);
                connect(m_dragDetectTimer, &QTimer::timeout, this, &TransparentDrawingWindow::onDragDetectionTimeout);
            }
            m_dragDetectTimer->start(2000);  // Wait 2 seconds after last check
            return;
        }
        
        // Use the smart function that only brings to front if needed
        bringWindowToFrontIfNeeded(reinterpret_cast<void *>(winId()));
#else
        // On other platforms, check if window is really behind
        if (QApplication::activeWindow() != this) {
            raise();
            activateWindow();
        }
#endif
    }
}

void TransparentDrawingWindow::onDragDetectionTimeout()
{
    // Called when we haven't seen window position changes for 2 seconds
    // This likely means the drag operation finished
    m_otherWindowBeingDragged = false;
    
    if (m_dragDetectTimer) {
        m_dragDetectTimer->stop();
    }
    
    // Now restore window to front since dragging is likely done
    if (isVisible()) {
#ifdef Q_OS_MACOS
        bringWindowToFrontIfNeeded(reinterpret_cast<void *>(winId()));
#endif
    }
}
