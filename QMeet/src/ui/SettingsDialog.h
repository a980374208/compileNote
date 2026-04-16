#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QTabWidget>
#include <QComboBox>
#include <QSlider>
#include <QCheckBox>
#include <QLineEdit>
#include <QPushButton>

/**
 * @brief 设置对话框
 * 提供应用程序设置界面
 */
class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = nullptr);
    ~SettingsDialog() override = default;

private slots:
    void onSave();
    void onCancel();
    void onReset();

private:
    void setupUI();
    void loadSettings();
    void saveSettings();

    // Audio settings
    QComboBox *m_audioInputCombo = nullptr;
    QComboBox *m_audioOutputCombo = nullptr;
    QSlider *m_audioVolumeSlider = nullptr;
    QCheckBox *m_noiseSuppressionCheck = nullptr;
    QCheckBox *m_echoCancellationCheck = nullptr;

    // Video settings
    QComboBox *m_videoInputCombo = nullptr;
    QComboBox *m_resolutionCombo = nullptr;
    QCheckBox *m_mirrorVideoCheck = nullptr;

    // Meeting settings
    QCheckBox *m_autoJoinAudioCheck = nullptr;
    QCheckBox *m_autoEnableVideoCheck = nullptr;
    QCheckBox *m_showNotificationsCheck = nullptr;
    QCheckBox *m_waitingRoomCheck = nullptr;

    // General settings
    QComboBox *m_languageCombo = nullptr;
    QComboBox *m_themeCombo = nullptr;
    QLineEdit *m_serverUrlEdit = nullptr;
};

#endif // SETTINGSDIALOG_H
