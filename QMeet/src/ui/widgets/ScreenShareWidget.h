#ifndef SCREENSHAREWIDGET_H
#define SCREENSHAREWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QImage>

class TransparentDrawingWindow;

/**
 * @brief 屏幕共享组件
 * 显示共享的屏幕内容，按比例撑满显示区域
 */
class ScreenShareWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ScreenShareWidget(QWidget *parent = nullptr);
    ~ScreenShareWidget() override = default;

    void setScreenImage(const QImage &image);
    void setSharerName(const QString &name);
    void setSharing(bool sharing);
    bool isSharing() const { return m_isSharing; }

signals:
    void stopSharing();
    void pauseSharing();
    void resumeSharing();

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    void setupUI();
    void setupDrawingButton();
    QRect calculateImageRect() const;
    void onDrawingButtonClicked();

    bool m_isSharing = false;
    QString m_sharerName;
    QImage m_currentImage;

    QLabel *m_sharerLabel = nullptr;
    QWidget *m_overlay = nullptr;
    QPushButton *m_stopBtn = nullptr;
    QPushButton *m_drawingBtn = nullptr;
    TransparentDrawingWindow *m_drawingWindow = nullptr;
};

#endif // SCREENSHAREWIDGET_H
