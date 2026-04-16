#ifndef PARTICIPANTWIDGET_H
#define PARTICIPANTWIDGET_H

#include <QWidget>
#include <QListView>
#include <QLabel>
#include <QVBoxLayout>
#include "../../models/User.h"
#include "../../models/ParticipantListModel.h"

class ParticipantListDelegate;

/**
 * @brief 参与者列表组件
 * 显示会议中的所有参与者
 */
class ParticipantWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ParticipantWidget(QWidget *parent = nullptr);
    ~ParticipantWidget() override = default;

    void addParticipant(User *user);
    void removeParticipant(const QString &userId);
    void clearParticipants();
    void updateParticipantCount(int count);

signals:
    void participantClicked(const QString &userId);
    void participantMuteRequested(const QString &userId);
    void participantCameraRequested(const QString &userId);
    void participantRemoveRequested(const QString &userId);

private slots:
    void onParticipantClicked(const QModelIndex &index);
    void onParticipantContextMenu(const QPoint &pos);

private:
    void setupUI();
    void setupConnections();
    void updateParticipantItem(User *user);
    void addSampleParticipants();

    QLabel *m_countLabel = nullptr;
    QListView *m_participantListView = nullptr;
    ParticipantListModel *m_participantListModel = nullptr;
    ParticipantListDelegate *m_participantListDelegate = nullptr;
};

#endif // PARTICIPANTWIDGET_H
