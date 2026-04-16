#include "SettingsDialog.h"
#include "../app/Application.h"
#include "../app/Settings.h"
#include "../core/MediaManager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QLabel>
#include <QDialogButtonBox>

SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent)
{
    setupUI();
    loadSettings();
}

void SettingsDialog::setupUI()
{
    setWindowTitle("设置");
    setMinimumSize(500, 400);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QTabWidget *tabWidget = new QTabWidget();

    // === Audio Tab ===
    QWidget *audioTab = new QWidget();
    QVBoxLayout *audioLayout = new QVBoxLayout(audioTab);

    QGroupBox *audioDevicesGroup = new QGroupBox("音频设备");
    QFormLayout *audioDevicesLayout = new QFormLayout(audioDevicesGroup);

    m_audioInputCombo = new QComboBox();
    m_audioOutputCombo = new QComboBox();
    m_audioVolumeSlider = new QSlider(Qt::Horizontal);
    m_audioVolumeSlider->setRange(0, 100);
    m_audioVolumeSlider->setValue(100);

    audioDevicesLayout->addRow("麦克风:", m_audioInputCombo);
    audioDevicesLayout->addRow("扬声器:", m_audioOutputCombo);
    audioDevicesLayout->addRow("音量:", m_audioVolumeSlider);

    QGroupBox *audioEnhanceGroup = new QGroupBox("音频增强");
    QVBoxLayout *audioEnhanceLayout = new QVBoxLayout(audioEnhanceGroup);

    m_noiseSuppressionCheck = new QCheckBox("启用降噪");
    m_echoCancellationCheck = new QCheckBox("启用回声消除");

    audioEnhanceLayout->addWidget(m_noiseSuppressionCheck);
    audioEnhanceLayout->addWidget(m_echoCancellationCheck);

    audioLayout->addWidget(audioDevicesGroup);
    audioLayout->addWidget(audioEnhanceGroup);
    audioLayout->addStretch();

    tabWidget->addTab(audioTab, "音频");

    // === Video Tab ===
    QWidget *videoTab = new QWidget();
    QVBoxLayout *videoLayout = new QVBoxLayout(videoTab);

    QGroupBox *videoDevicesGroup = new QGroupBox("视频设备");
    QFormLayout *videoDevicesLayout = new QFormLayout(videoDevicesGroup);

    m_videoInputCombo = new QComboBox();
    m_resolutionCombo = new QComboBox();
    m_resolutionCombo->addItems({"640x480", "1280x720", "1920x1080"});

    videoDevicesLayout->addRow("摄像头:", m_videoInputCombo);
    videoDevicesLayout->addRow("分辨率:", m_resolutionCombo);

    QGroupBox *videoOptionsGroup = new QGroupBox("视频选项");
    QVBoxLayout *videoOptionsLayout = new QVBoxLayout(videoOptionsGroup);

    m_mirrorVideoCheck = new QCheckBox("镜像视频");

    videoOptionsLayout->addWidget(m_mirrorVideoCheck);

    videoLayout->addWidget(videoDevicesGroup);
    videoLayout->addWidget(videoOptionsGroup);
    videoLayout->addStretch();

    tabWidget->addTab(videoTab, "视频");

    // === Meeting Tab ===
    QWidget *meetingTab = new QWidget();
    QVBoxLayout *meetingLayout = new QVBoxLayout(meetingTab);

    QGroupBox *meetingOptionsGroup = new QGroupBox("会议选项");
    QVBoxLayout *meetingOptionsLayout = new QVBoxLayout(meetingOptionsGroup);

    m_autoJoinAudioCheck = new QCheckBox("自动加入音频");
    m_autoEnableVideoCheck = new QCheckBox("自动开启视频");
    m_showNotificationsCheck = new QCheckBox("显示通知");
    m_waitingRoomCheck = new QCheckBox("启用等候室");

    meetingOptionsLayout->addWidget(m_autoJoinAudioCheck);
    meetingOptionsLayout->addWidget(m_autoEnableVideoCheck);
    meetingOptionsLayout->addWidget(m_showNotificationsCheck);
    meetingOptionsLayout->addWidget(m_waitingRoomCheck);

    meetingLayout->addWidget(meetingOptionsGroup);
    meetingLayout->addStretch();

    tabWidget->addTab(meetingTab, "会议");

    // === General Tab ===
    QWidget *generalTab = new QWidget();
    QVBoxLayout *generalLayout = new QVBoxLayout(generalTab);

    QGroupBox *generalSettingsGroup = new QGroupBox("常规设置");
    QFormLayout *generalSettingsLayout = new QFormLayout(generalSettingsGroup);

    m_languageCombo = new QComboBox();
    m_languageCombo->addItems({"简体中文", "English"});

    m_themeCombo = new QComboBox();
    m_themeCombo->addItems({"浅色", "深色", "跟随系统"});

    m_serverUrlEdit = new QLineEdit();

    generalSettingsLayout->addRow("语言:", m_languageCombo);
    generalSettingsLayout->addRow("主题:", m_themeCombo);
    generalSettingsLayout->addRow("服务器地址:", m_serverUrlEdit);

    generalLayout->addWidget(generalSettingsGroup);
    generalLayout->addStretch();

    tabWidget->addTab(generalTab, "常规");

    mainLayout->addWidget(tabWidget);

    // Buttons
    QDialogButtonBox *buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Save | QDialogButtonBox::Cancel | QDialogButtonBox::Reset);
    
    connect(buttonBox, &QDialogButtonBox::accepted, this, &SettingsDialog::onSave);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &SettingsDialog::onCancel);
    connect(buttonBox->button(QDialogButtonBox::Reset), &QPushButton::clicked, this, &SettingsDialog::onReset);

    mainLayout->addWidget(buttonBox);

    // Populate device lists
    MediaManager mediaManager;
    m_audioInputCombo->addItems(mediaManager.availableAudioInputs());
    m_audioOutputCombo->addItems(mediaManager.availableAudioOutputs());
    m_videoInputCombo->addItems(mediaManager.availableCameras());
}

void SettingsDialog::loadSettings()
{
    Settings *settings = Application::instance()->settings();

    // Audio
    m_audioVolumeSlider->setValue(settings->audioVolume());
    m_autoJoinAudioCheck->setChecked(settings->autoJoinAudio());

    // Video
    m_autoEnableVideoCheck->setChecked(settings->autoEnableVideo());

    // Meeting
    m_showNotificationsCheck->setChecked(settings->showNotifications());

    // General
    int langIndex = settings->language() == "zh_CN" ? 0 : 1;
    m_languageCombo->setCurrentIndex(langIndex);
    
    int themeIndex = settings->theme() == "light" ? 0 : (settings->theme() == "dark" ? 1 : 2);
    m_themeCombo->setCurrentIndex(themeIndex);
    
    m_serverUrlEdit->setText(settings->serverUrl());
}

void SettingsDialog::saveSettings()
{
    Settings *settings = Application::instance()->settings();

    // Audio
    settings->setAudioVolume(m_audioVolumeSlider->value());
    settings->setAutoJoinAudio(m_autoJoinAudioCheck->isChecked());

    // Video
    settings->setAutoEnableVideo(m_autoEnableVideoCheck->isChecked());

    // Meeting
    settings->setShowNotifications(m_showNotificationsCheck->isChecked());

    // General
    QString lang = m_languageCombo->currentIndex() == 0 ? "zh_CN" : "en_US";
    settings->setLanguage(lang);

    QStringList themes = {"light", "dark", "system"};
    settings->setTheme(themes[m_themeCombo->currentIndex()]);

    settings->setServerUrl(m_serverUrlEdit->text());
}

void SettingsDialog::onSave()
{
    saveSettings();
    accept();
}

void SettingsDialog::onCancel()
{
    reject();
}

void SettingsDialog::onReset()
{
    // Reset to default values
    m_audioVolumeSlider->setValue(100);
    m_autoJoinAudioCheck->setChecked(true);
    m_autoEnableVideoCheck->setChecked(true);
    m_showNotificationsCheck->setChecked(true);
    m_languageCombo->setCurrentIndex(0);
    m_themeCombo->setCurrentIndex(0);
    m_serverUrlEdit->setText("wss://meet.example.com");
}
