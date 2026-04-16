#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QListView>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QStackedWidget>
#include <QTimer>
#include "../models/Meeting.h"
#include "../models/MeetingListModel.h"

class MeetingListDelegate;

/**
 * @brief 主窗口
 * 显示会议列表、用户信息，支持创建/加入会议
 */
class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override = default;

    void setCurrentUser(User *user);

signals:
    void logoutRequested();
    void joinMeetingRequested(const QString &meetingId);
    void createMeetingRequested(const QString &title);
    void settingsRequested();

private slots:
    void onCreateMeetingClicked();
    void onJoinMeetingClicked();
    void onScheduleMeetingClicked();
    void onQuickMeetingClicked();
    void onMeetingItemDoubleClicked(const QModelIndex &index);
    void onLogoutClicked();
    void onSettingsClicked();
    void refreshMeetingList();

private:
    void setupUI();
    void setupConnections();
    void loadMeetings();
    void updateUserInfo();

    User *m_currentUser = nullptr;

    // Header widgets
    QLabel *m_avatarLabel = nullptr;
    QLabel *m_userNameLabel = nullptr;
    QLabel *m_statusLabel = nullptr;
    QPushButton *m_settingsBtn = nullptr;
    QPushButton *m_logoutBtn = nullptr;

    // Quick actions
    QPushButton *m_newMeetingBtn = nullptr;
    QPushButton *m_joinMeetingBtn = nullptr;
    QPushButton *m_scheduleBtn = nullptr;

    // Meeting code input
    QLineEdit *m_meetingCodeEdit = nullptr;

    // Meeting list
    QListView *m_meetingListView = nullptr;
    MeetingListModel *m_meetingListModel = nullptr;
    MeetingListDelegate *m_meetingListDelegate = nullptr;

    // Timer for refreshing meeting list
    QTimer *m_refreshTimer = nullptr;
};

#endif // MAINWINDOW_H
