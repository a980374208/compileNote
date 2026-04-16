#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QCheckBox>
#include <QStackedWidget>

/**
 * @brief 登录窗口
 * 提供用户登录和注册功能
 */
class LoginWindow : public QWidget
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow() override = default;

signals:
    void loginSuccess(const QString &userId, const QString &token);
    void registerRequested();

private slots:
    void onLoginClicked();
    void onRegisterClicked();
    void onForgotPasswordClicked();
    void onThirdPartyLogin(const QString &provider);

private:
    void setupUI();
    void setupConnections();
    bool validateInput();
    void showLoginPanel();
    void showRegisterPanel();

    QStackedWidget *m_stackedWidget = nullptr;

    // Login panel widgets
    QLineEdit *m_usernameEdit = nullptr;
    QLineEdit *m_passwordEdit = nullptr;
    QCheckBox *m_rememberMeCheck = nullptr;
    QPushButton *m_loginBtn = nullptr;
    QPushButton *m_registerBtn = nullptr;

    // Register panel widgets
    QLineEdit *m_regUsernameEdit = nullptr;
    QLineEdit *m_regEmailEdit = nullptr;
    QLineEdit *m_regPasswordEdit = nullptr;
    QLineEdit *m_regConfirmPasswordEdit = nullptr;
    QPushButton *m_regSubmitBtn = nullptr;
    QPushButton *m_regBackBtn = nullptr;
};

#endif // LOGINWINDOW_H
