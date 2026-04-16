#include "Application.h"
#include "Settings.h"
#include "../ui/LoginWindow.h"
#include "../ui/MainWindow.h"
#include "../ui/MeetingRoomWindow.h"
#include "../models/User.h"
#include <QFile>
#include <QStyle>
#include <QScreen>
#include <QRect>

Application::Application(int &argc, char **argv)
    : QApplication(argc, argv)
    , m_settings(std::make_unique<Settings>())
{
    setApplicationName("QMeet");
    setApplicationVersion("1.0.0");
    setOrganizationName("QMeet");

    // Set application style
    setStyle("Fusion");
}

Application::~Application()
{
    delete m_currentUser;
}

Application* Application::instance()
{
    return qobject_cast<Application*>(QApplication::instance());
}

User* Application::currentUser() const
{
    return m_currentUser;
}

void Application::setCurrentUser(User *user)
{
    if (m_currentUser) {
        delete m_currentUser;
    }
    m_currentUser = user;
    emit userLoggedIn(user);
}

Settings* Application::settings() const
{
    return m_settings.get();
}

bool Application::initialize()
{
    // Create main frame
    m_mainFrame = new QMainWindow();
    m_mainFrame->setWindowTitle("QMeet - 视频会议");
    m_mainFrame->setMinimumSize(1200, 800);

    // Center the window
    QScreen *screen = primaryScreen();
    QRect screenGeometry = screen->availableGeometry();
    int x = (screenGeometry.width() - m_mainFrame->width()) / 2;
    int y = (screenGeometry.height() - m_mainFrame->height()) / 2;
    m_mainFrame->move(x, y);

    // Create stacked widget for navigation
    m_stackedWidget = new QStackedWidget(m_mainFrame);
    m_mainFrame->setCentralWidget(m_stackedWidget);

    // Create windows
    m_loginWindow = new LoginWindow();
    m_homeWindow = new MainWindow();
    m_meetingRoomWindow = new MeetingRoomWindow();

    m_stackedWidget->addWidget(m_loginWindow);
    m_stackedWidget->addWidget(m_homeWindow);
    m_stackedWidget->addWidget(m_meetingRoomWindow);

    // Setup connections
    setupConnections();

    // Load stylesheet
    loadStylesheet();

    // Show login window by default
    showLogin();

    // Show main window
    m_mainFrame->show();

    return true;
}

void Application::showLogin()
{
    m_stackedWidget->setCurrentWidget(m_loginWindow);
    m_mainFrame->setWindowTitle("QMeet - 登录");
}

void Application::showMainWindow()
{
    m_stackedWidget->setCurrentWidget(m_homeWindow);
    m_mainFrame->setWindowTitle("QMeet - 主页");
}

void Application::showMeetingRoom()
{
    m_stackedWidget->setCurrentWidget(m_meetingRoomWindow);
    m_mainFrame->setWindowTitle("QMeet - 会议中");
}

void Application::logout()
{
    m_authToken.clear();
    emit userLoggedOut();
    showLogin();
}

void Application::setupConnections()
{
    connect(m_loginWindow, &LoginWindow::loginSuccess,
            this, &Application::onLoginSuccess);

    connect(m_homeWindow, &MainWindow::logoutRequested,
            this, &Application::onLogoutRequested);

    connect(m_homeWindow, &MainWindow::joinMeetingRequested,
            this, &Application::showMeetingRoom);

    connect(m_meetingRoomWindow, &MeetingRoomWindow::leaveMeeting,
            this, &Application::showMainWindow);
}

void Application::loadStylesheet()
{
    QFile styleFile(":/styles/main.qss");
    if (styleFile.open(QFile::ReadOnly)) {
        QString styleSheet = QString::fromUtf8(styleFile.readAll());
        setStyleSheet(styleSheet);
        styleFile.close();
    }
}

void Application::onLoginSuccess(const QString &userId, const QString &token)
{
    Q_UNUSED(userId);
    m_authToken = token;
    showMainWindow();
}

void Application::onLogoutRequested()
{
    logout();
}
