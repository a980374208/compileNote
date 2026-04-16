#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>
#include <QMainWindow>
#include <QStackedWidget>
#include <memory>

class User;
class Settings;
class LoginWindow;
class MainWindow;
class MeetingRoomWindow;
class NetworkManager;

/**
 * @brief 应用程序主类
 * 管理应用程序的生命周期和主要窗口
 */
class Application : public QApplication
{
    Q_OBJECT

public:
    Application(int &argc, char **argv);
    ~Application() override;

    static Application* instance();

    // User management
    User* currentUser() const;
    void setCurrentUser(User *user);

    // Settings
    Settings* settings() const;

    // Navigation
    void showLogin();
    void showMainWindow();
    void showMeetingRoom();
    void logout();

    // Initialization
    bool initialize();

signals:
    void userLoggedIn(User *user);
    void userLoggedOut();

private slots:
    void onLoginSuccess(const QString &userId, const QString &token);
    void onLogoutRequested();

private:
    void setupConnections();
    void loadStylesheet();

    std::unique_ptr<Settings> m_settings;
    User *m_currentUser = nullptr;
    QString m_authToken;

    QMainWindow *m_mainFrame = nullptr;
    QStackedWidget *m_stackedWidget = nullptr;
    LoginWindow *m_loginWindow = nullptr;
    MainWindow *m_homeWindow = nullptr;
    MeetingRoomWindow *m_meetingRoomWindow = nullptr;
};

#endif // APPLICATION_H
