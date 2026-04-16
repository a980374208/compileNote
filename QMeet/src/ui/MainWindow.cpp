#include "MainWindow.h"
#include "delegates/MeetingListDelegate.h"
#include "../app/Application.h"
#include "../models/User.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QInputDialog>
#include <QDateTime>
#include <QScrollArea>
#include <QPainter>
#include <QListView>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
    setupConnections();

    // Start refresh timer
    m_refreshTimer = new QTimer(this);
    connect(m_refreshTimer, &QTimer::timeout, this, &MainWindow::refreshMeetingList);
    m_refreshTimer->start(30000); // Refresh every 30 seconds
}

void MainWindow::setCurrentUser(User *user)
{
    m_currentUser = user;
    updateUserInfo();
}

void MainWindow::setupUI()
{
    setObjectName("MainWindow");

    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // === Left Sidebar ===
    QWidget *sidebar = new QWidget();
    sidebar->setObjectName("mainSidebar");
    sidebar->setFixedWidth(240);
    QVBoxLayout *sidebarLayout = new QVBoxLayout(sidebar);
    sidebarLayout->setContentsMargins(16, 16, 16, 16);
    sidebarLayout->setSpacing(16);

    // User info section
    QWidget *userSection = new QWidget();
    QVBoxLayout *userLayout = new QVBoxLayout(userSection);
    userLayout->setSpacing(8);

    m_avatarLabel = new QLabel();
    m_avatarLabel->setObjectName("avatarLabel");
    m_avatarLabel->setFixedSize(64, 64);
    m_avatarLabel->setAlignment(Qt::AlignCenter);
    m_avatarLabel->setText("U");

    m_userNameLabel = new QLabel("用户名");

    m_statusLabel = new QLabel("在线");

    userLayout->addWidget(m_avatarLabel, 0, Qt::AlignCenter);
    userLayout->addWidget(m_userNameLabel, 0, Qt::AlignCenter);
    userLayout->addWidget(m_statusLabel, 0, Qt::AlignCenter);

    sidebarLayout->addWidget(userSection);

    // Sidebar buttons
    auto createSidebarBtn = [](const QString &text, const QString &icon) -> QPushButton* {
        QPushButton *btn = new QPushButton(icon + " " + text);
        btn->setCursor(Qt::PointingHandCursor);
        return btn;
    };

    m_newMeetingBtn = createSidebarBtn("新建会议", "🎥");
    m_joinMeetingBtn = createSidebarBtn("加入会议", "➡️");
    m_scheduleBtn = createSidebarBtn("预定会议", "📅");

    sidebarLayout->addWidget(m_newMeetingBtn);
    sidebarLayout->addWidget(m_joinMeetingBtn);
    sidebarLayout->addWidget(m_scheduleBtn);
    sidebarLayout->addStretch();

    // Settings and logout
    m_settingsBtn = createSidebarBtn("设置", "⚙️");
    m_logoutBtn = createSidebarBtn("退出登录", "🚪");

    sidebarLayout->addWidget(m_settingsBtn);
    sidebarLayout->addWidget(m_logoutBtn);

    mainLayout->addWidget(sidebar);

    // === Main Content Area ===
    QWidget *contentArea = new QWidget();
    QVBoxLayout *contentLayout = new QVBoxLayout(contentArea);
    contentLayout->setContentsMargins(32, 32, 32, 32);
    contentLayout->setSpacing(24);

    // Header
    QLabel *welcomeLabel = new QLabel("欢迎使用 QMeet");
    welcomeLabel->setObjectName("welcomeLabel");
    contentLayout->addWidget(welcomeLabel);

    // Quick join section
    QWidget *quickJoinSection = new QWidget();
    QHBoxLayout *quickJoinLayout = new QHBoxLayout(quickJoinSection);
    quickJoinLayout->setSpacing(16);

    QLabel *joinLabel = new QLabel("会议号:");
    joinLabel->setObjectName("joinLabel");

    m_meetingCodeEdit = new QLineEdit();
    m_meetingCodeEdit->setObjectName("meetingCodeEdit");
    m_meetingCodeEdit->setPlaceholderText("请输入会议号");
    m_meetingCodeEdit->setMinimumHeight(44);

    QPushButton *quickJoinBtn = new QPushButton("加入");
    quickJoinBtn->setObjectName("quickJoinBtn");
    quickJoinBtn->setFixedSize(100, 44);

    quickJoinLayout->addWidget(joinLabel);
    quickJoinLayout->addWidget(m_meetingCodeEdit, 1);
    quickJoinLayout->addWidget(quickJoinBtn);
    contentLayout->addWidget(quickJoinSection);

    connect(quickJoinBtn, &QPushButton::clicked, this, &MainWindow::onJoinMeetingClicked);

    // Meeting list section
    QLabel *meetingListLabel = new QLabel("我的会议");
    meetingListLabel->setObjectName("meetingListLabel");
    contentLayout->addWidget(meetingListLabel);

    m_meetingListModel = new MeetingListModel(this);
    m_meetingListDelegate = new MeetingListDelegate(this);

    m_meetingListView = new QListView();
    m_meetingListView->setModel(m_meetingListModel);
    m_meetingListView->setItemDelegate(m_meetingListDelegate);
    m_meetingListView->setSpacing(4);
    contentLayout->addWidget(m_meetingListView, 1);

    // Load sample meetings
    loadMeetings();

    mainLayout->addWidget(contentArea, 1);
}

void MainWindow::setupConnections()
{
    connect(m_newMeetingBtn, &QPushButton::clicked, this, &MainWindow::onCreateMeetingClicked);
    connect(m_joinMeetingBtn, &QPushButton::clicked, this, &MainWindow::onJoinMeetingClicked);
    connect(m_scheduleBtn, &QPushButton::clicked, this, &MainWindow::onScheduleMeetingClicked);
    connect(m_settingsBtn, &QPushButton::clicked, this, &MainWindow::onSettingsClicked);
    connect(m_logoutBtn, &QPushButton::clicked, this, &MainWindow::onLogoutClicked);
    connect(m_meetingListView, &QListView::doubleClicked, this, &MainWindow::onMeetingItemDoubleClicked);
}

void MainWindow::loadMeetings()
{
    m_meetingListModel->clear();

    // Sample meetings
    QList<MeetingInfo> meetings = {
        MeetingInfo{"123456789", "项目周会", "2024-01-15 10:00", "张三", 5, false},
        MeetingInfo{"987654321", "产品评审", "2024-01-16 14:00", "李四", 8, true},
        MeetingInfo{"456789123", "技术分享", "2024-01-17 15:30", "王五", 12, false}
    };

    for (const MeetingInfo &meeting : meetings) {
        m_meetingListModel->addMeeting(meeting);
    }
}

void MainWindow::updateUserInfo()
{
    if (m_currentUser) {
        m_userNameLabel->setText(m_currentUser->name());
        m_statusLabel->setText(m_currentUser->statusText());
    // Status color is applied through stylesheet

        // Set avatar initial
        if (!m_currentUser->name().isEmpty()) {
            m_avatarLabel->setText(m_currentUser->name().at(0).toUpper());
        }
    }
}

void MainWindow::onCreateMeetingClicked()
{
    QString title = QInputDialog::getText(this, "新建会议", "请输入会议主题:");
    if (!title.isEmpty()) {
        emit createMeetingRequested(title);
    }
}

void MainWindow::onJoinMeetingClicked()
{
    QString meetingCode = m_meetingCodeEdit->text().trimmed();
    if (meetingCode.isEmpty()) {
        meetingCode = QInputDialog::getText(this, "加入会议", "请输入会议号:");
    }

    if (!meetingCode.isEmpty()) {
        emit joinMeetingRequested(meetingCode);
    }
}

void MainWindow::onScheduleMeetingClicked()
{
    QMessageBox::information(this, "预定会议", "预定会议功能正在开发中...");
}

void MainWindow::onQuickMeetingClicked()
{
    emit createMeetingRequested("快速会议");
}

void MainWindow::onMeetingItemDoubleClicked(const QModelIndex &index)
{
    if (!index.isValid()) {
        return;
    }
    MeetingInfo meeting = m_meetingListModel->getMeeting(index.row());
    emit joinMeetingRequested(meeting.id);
}

void MainWindow::onLogoutClicked()
{
    auto reply = QMessageBox::question(this, "退出登录", "确定要退出登录吗?",
                                       QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        emit logoutRequested();
    }
}

void MainWindow::onSettingsClicked()
{
    emit settingsRequested();
}

void MainWindow::refreshMeetingList()
{
    // TODO: Implement actual refresh logic
    loadMeetings();
}
