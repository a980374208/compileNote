#ifndef TRANSPARENTDRAWINGWINDOW_H
#define TRANSPARENTDRAWINGWINDOW_H

#include <QWidget>
#include <QVector>
#include <QPoint>
#include <QColor>
#include <QTimer>

/**
 * @brief 透明绘图窗口
 * 覆盖整个屏幕的透明窗口，用于在屏幕共享上进行绘制
 * 鼠标事件透传给下方窗口
 */
class TransparentDrawingWindow : public QWidget
{
    Q_OBJECT

public:
    explicit TransparentDrawingWindow(QWidget *parent = nullptr);
    ~TransparentDrawingWindow() override = default;

    void setDrawingColor(const QColor &color) { m_drawingColor = color; }
    void setDrawingLineWidth(int width) { m_drawingLineWidth = width; }
    void clearDrawings();
    
    void enableDrawing(bool enabled) { m_drawingEnabled = enabled; }
    void bringToFront();  // Bring the drawing window to the front on macOS

signals:
    void drawingFinished();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void showEvent(QShowEvent *event) override;  // Bring to front when shown

private:
    bool m_drawingEnabled = true;
    bool m_isDrawing = false;
    QVector<QVector<QPoint>> m_drawings;  // List of strokes
    QVector<QPoint> m_currentStroke;
    QColor m_drawingColor = Qt::red;
    int m_drawingLineWidth = 2;
    
    // Focus pass-through control
    bool m_focusPassthrough = false;  // Whether mouse events pass through to lower windows
    void setFocusPassthrough(bool enable);  // Set focus pass-through state and update cursor
    void updateCursor();  // Update cursor based on pass-through state
    void enableMouseEventPassthrough(bool enable);  // Enable/disable macOS mouse event passthrough
    
    // Window Z-order monitoring
    QTimer *m_windowOrderTimer = nullptr;  // Timer to check window z-order
    QTimer *m_dragDetectTimer = nullptr;   // Timer to detect when other windows stop being dragged
    bool m_otherWindowBeingDragged = false; // Flag to track if other windows are being dragged
    void initWindowOrderMonitoring();  // Initialize window order monitoring
    
private slots:
    void checkAndRestoreWindowOrder();  // Check if window is still in front
    void onDragDetectionTimeout();      // Called when drag operation likely finished
};

#endif // TRANSPARENTDRAWINGWINDOW_H
