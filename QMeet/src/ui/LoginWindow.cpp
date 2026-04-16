#include "LoginWindow.h"
#include "../app/Application.h"
#include "../app/Settings.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QSpacerItem>
#include <QRegularExpression>
#include <QUuid>

LoginWindow::LoginWindow(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
    setupConnections();

    // Load saved credentials
    Settings *settings = Application::instance()->settings();
    if (settings->rememberMe()) {
        m_usernameEdit->setText(settings->lastUsername());
        m_rememberMeCheck->setChecked(true);
    }
}

void LoginWindow::setupUI()
{
    setStyleSheet(R"(
        QWidget {
            background-color: white;
        }
        QLineEdit {
            padding: 12px 16px;
            border: 1px solid #e0e0e0;
            border-radius: 8px;
            font-size: 14px;
            background-color: #fafafa;
        }
        QLineEdit:focus {
            border: 2px solid #0078d4;
            background-color: white;
        }
        QPushButton {
            padding: 12px 24px;
            border-radius: 8px;
            font-size: 14px;
            font-weight: bold;
        }
        QLabel {
            color: #333333;
        }
    )");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // Center container
    QWidget *centerWidget = new QWidget();
    centerWidget->setMaximumWidth(420);
    mainLayout->addStretch();
    mainLayout->addWidget(centerWidget, 0, Qt::AlignCenter);
    mainLayout->addStretch();

    QVBoxLayout *centerLayout = new QVBoxLayout(centerWidget);
    centerLayout->setSpacing(24);

    // Logo and title
    QLabel *logoLabel = new QLabel();
    logoLabel->setAlignment(Qt::AlignCenter);
    logoLabel->setText("🎬");  // Placeholder for logo
    logoLabel->setStyleSheet("font-size: 48px;");

    QLabel *titleLabel = new QLabel("QMeet");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 32px; font-weight: bold; color: #0078d4;");

    QLabel *subtitleLabel = new QLabel("安全、稳定的视频会议平台");
    subtitleLabel->setAlignment(Qt::AlignCenter);
    subtitleLabel->setStyleSheet("font-size: 14px; color: #666666;");

    centerLayout->addWidget(logoLabel);
    centerLayout->addWidget(titleLabel);
    centerLayout->addWidget(subtitleLabel);
    centerLayout->addSpacing(16);

    // Stacked widget for login/register panels
    m_stackedWidget = new QStackedWidget();
    centerLayout->addWidget(m_stackedWidget);

    // === Login Panel ===
    QWidget *loginPanel = new QWidget();
    QVBoxLayout *loginLayout = new QVBoxLayout(loginPanel);
    loginLayout->setSpacing(16);

    // Username
    m_usernameEdit = new QLineEdit();
    m_usernameEdit->setPlaceholderText("请输入用户名 / 手机号 / 邮箱");
    m_usernameEdit->setMinimumHeight(48);
    loginLayout->addWidget(m_usernameEdit);

    // Password
    m_passwordEdit = new QLineEdit();
    m_passwordEdit->setPlaceholderText("请输入密码");
    m_passwordEdit->setEchoMode(QLineEdit::Password);
    m_passwordEdit->setMinimumHeight(48);
    m_passwordEdit->setText("aa");
    loginLayout->addWidget(m_passwordEdit);

    // Remember me and forgot password
    QHBoxLayout *optionsLayout = new QHBoxLayout();
    m_rememberMeCheck = new QCheckBox("记住我");
    QPushButton *forgotBtn = new QPushButton("忘记密码？");
    forgotBtn->setFlat(true);
    forgotBtn->setStyleSheet("color: #0078d4; background: transparent; padding: 0;");
    optionsLayout->addWidget(m_rememberMeCheck);
    optionsLayout->addStretch();
    optionsLayout->addWidget(forgotBtn);
    loginLayout->addLayout(optionsLayout);

    // Login button
    m_loginBtn = new QPushButton("登 录");
    m_loginBtn->setMinimumHeight(48);
    m_loginBtn->setStyleSheet(R"(
        QPushButton {
            background-color: #0078d4;
            color: white;
            border: none;
        }
        QPushButton:hover {
            background-color: #106ebe;
        }
        QPushButton:pressed {
            background-color: #005a9e;
        }
    )");
    loginLayout->addWidget(m_loginBtn);

    // Register link
    QHBoxLayout *registerLayout = new QHBoxLayout();
    QLabel *noAccountLabel = new QLabel("没有账号？");
    noAccountLabel->setStyleSheet("color: #666666;");
    m_registerBtn = new QPushButton("立即注册");
    m_registerBtn->setFlat(true);
    m_registerBtn->setStyleSheet("color: #0078d4; background: transparent; padding: 0; font-weight: bold;");
    registerLayout->addStretch();
    registerLayout->addWidget(noAccountLabel);
    registerLayout->addWidget(m_registerBtn);
    registerLayout->addStretch();
    loginLayout->addLayout(registerLayout);

    // Divider
    loginLayout->addSpacing(16);
    QHBoxLayout *dividerLayout = new QHBoxLayout();
    QFrame *line1 = new QFrame();
    line1->setFrameShape(QFrame::HLine);
    line1->setStyleSheet("background-color: #e0e0e0;");
    QLabel *orLabel = new QLabel("或");
    orLabel->setAlignment(Qt::AlignCenter);
    orLabel->setStyleSheet("color: #999999; padding: 0 16px;");
    QFrame *line2 = new QFrame();
    line2->setFrameShape(QFrame::HLine);
    line2->setStyleSheet("background-color: #e0e0e0;");
    dividerLayout->addWidget(line1);
    dividerLayout->addWidget(orLabel);
    dividerLayout->addWidget(line2);
    loginLayout->addLayout(dividerLayout);

    // Third-party login
    loginLayout->addSpacing(8);
    QHBoxLayout *thirdPartyLayout = new QHBoxLayout();
    thirdPartyLayout->addStretch();
    
    QPushButton *wechatBtn = new QPushButton("微信");
    wechatBtn->setFixedSize(48, 48);
    wechatBtn->setStyleSheet("background-color: #07c160; color: white; border-radius: 24px; font-size: 12px;");
    wechatBtn->setProperty("provider", "wechat");

    QPushButton *qqBtn = new QPushButton("QQ");
    qqBtn->setFixedSize(48, 48);
    qqBtn->setStyleSheet("background-color: #12b7f5; color: white; border-radius: 24px; font-size: 12px;");
    qqBtn->setProperty("provider", "qq");

    thirdPartyLayout->addWidget(wechatBtn);
    thirdPartyLayout->addSpacing(16);
    thirdPartyLayout->addWidget(qqBtn);
    thirdPartyLayout->addStretch();
    loginLayout->addLayout(thirdPartyLayout);

    m_stackedWidget->addWidget(loginPanel);

    // === Register Panel ===
    QWidget *registerPanel = new QWidget();
    QVBoxLayout *regLayout = new QVBoxLayout(registerPanel);
    regLayout->setSpacing(16);

    m_regUsernameEdit = new QLineEdit();
    m_regUsernameEdit->setPlaceholderText("请输入用户名");
    m_regUsernameEdit->setMinimumHeight(48);
    regLayout->addWidget(m_regUsernameEdit);

    m_regEmailEdit = new QLineEdit();
    m_regEmailEdit->setPlaceholderText("请输入邮箱");
    m_regEmailEdit->setMinimumHeight(48);
    regLayout->addWidget(m_regEmailEdit);

    m_regPasswordEdit = new QLineEdit();
    m_regPasswordEdit->setPlaceholderText("请输入密码");
    m_regPasswordEdit->setEchoMode(QLineEdit::Password);
    m_regPasswordEdit->setMinimumHeight(48);
    regLayout->addWidget(m_regPasswordEdit);

    m_regConfirmPasswordEdit = new QLineEdit();
    m_regConfirmPasswordEdit->setPlaceholderText("请确认密码");
    m_regConfirmPasswordEdit->setEchoMode(QLineEdit::Password);
    m_regConfirmPasswordEdit->setMinimumHeight(48);
    regLayout->addWidget(m_regConfirmPasswordEdit);

    m_regSubmitBtn = new QPushButton("注 册");
    m_regSubmitBtn->setMinimumHeight(48);
    m_regSubmitBtn->setStyleSheet(R"(
        QPushButton {
            background-color: #0078d4;
            color: white;
            border: none;
        }
        QPushButton:hover {
            background-color: #106ebe;
        }
    )");
    regLayout->addWidget(m_regSubmitBtn);

    m_regBackBtn = new QPushButton("返回登录");
    m_regBackBtn->setFlat(true);
    m_regBackBtn->setStyleSheet("color: #0078d4; background: transparent;");
    QHBoxLayout *backLayout = new QHBoxLayout();
    backLayout->addStretch();
    backLayout->addWidget(m_regBackBtn);
    backLayout->addStretch();
    regLayout->addLayout(backLayout);

    m_stackedWidget->addWidget(registerPanel);

    // Connect third-party buttons
    connect(wechatBtn, &QPushButton::clicked, this, [this]() { onThirdPartyLogin("wechat"); });
    connect(qqBtn, &QPushButton::clicked, this, [this]() { onThirdPartyLogin("qq"); });
    connect(forgotBtn, &QPushButton::clicked, this, &LoginWindow::onForgotPasswordClicked);
}

void LoginWindow::setupConnections()
{
    connect(m_loginBtn, &QPushButton::clicked, this, &LoginWindow::onLoginClicked);
    connect(m_registerBtn, &QPushButton::clicked, this, &LoginWindow::onRegisterClicked);
    connect(m_regSubmitBtn, &QPushButton::clicked, this, &LoginWindow::showLoginPanel);
    connect(m_regBackBtn, &QPushButton::clicked, this, &LoginWindow::showLoginPanel);

    // Enter key to login
    connect(m_passwordEdit, &QLineEdit::returnPressed, this, &LoginWindow::onLoginClicked);
}

void LoginWindow::onLoginClicked()
{
    if (!validateInput()) {
        return;
    }

    QString username = m_usernameEdit->text().trimmed();
    QString password = m_passwordEdit->text();

    // Save credentials if remember me is checked
    Settings *settings = Application::instance()->settings();
    settings->setRememberMe(m_rememberMeCheck->isChecked());
    if (m_rememberMeCheck->isChecked()) {
        settings->setLastUsername(username);
    }

    // TODO: Implement actual login logic with backend
    // For demo, just emit success signal
    QString userId = QUuid::createUuid().toString(QUuid::WithoutBraces);
    QString token = "demo_token_" + userId;

    emit loginSuccess(userId, token);
}

void LoginWindow::onRegisterClicked()
{
    m_stackedWidget->setCurrentIndex(1);
}

void LoginWindow::onForgotPasswordClicked()
{
    QMessageBox::information(this, "忘记密码", "请联系管理员重置密码");
}

void LoginWindow::onThirdPartyLogin(const QString &provider)
{
    QMessageBox::information(this, "第三方登录", 
        QString("%1登录功能正在开发中...").arg(provider == "wechat" ? "微信" : "QQ"));
}

bool LoginWindow::validateInput()
{
    QString username = m_usernameEdit->text().trimmed();
    QString password = m_passwordEdit->text();

    if (username.isEmpty()) {
        m_usernameEdit->setFocus();
        m_usernameEdit->setStyleSheet(m_usernameEdit->styleSheet() + "border: 1px solid #d13438;");
        return false;
    }

    if (password.isEmpty()) {
        m_passwordEdit->setFocus();
        m_passwordEdit->setStyleSheet(m_passwordEdit->styleSheet() + "border: 1px solid #d13438;");
        return false;
    }

    return true;
}

void LoginWindow::showLoginPanel()
{
    m_stackedWidget->setCurrentIndex(0);
}

void LoginWindow::showRegisterPanel()
{
    m_stackedWidget->setCurrentIndex(1);
}
