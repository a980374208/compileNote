#include "ParticipantWidget.h"
#include "../delegates/ParticipantListDelegate.h"
#include <QHBoxLayout>
#include <QMenu>
#include <QAction>
#include <QLabel>
#include <QListView>

ParticipantWidget::ParticipantWidget(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
    setupConnections();
}

void ParticipantWidget::addParticipant(User *user)
{
    if (!user) {
        return;
    }
    m_participantListModel->addParticipant(user);
    updateParticipantCount(m_participantListModel->getParticipantCount());
}

void ParticipantWidget::removeParticipant(const QString &userId)
{
    m_participantListModel->removeParticipant(userId);
    updateParticipantCount(m_participantListModel->getParticipantCount());
}

void ParticipantWidget::clearParticipants()
{
    m_participantListModel->clear();
    updateParticipantCount(0);
}

void ParticipantWidget::updateParticipantCount(int count)
{
    m_countLabel->setText(QString("参与者 (%1)").arg(count));
}

void ParticipantWidget::setupUI()
{
    setObjectName("ParticipantWidget");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // Header
    QWidget *header = new QWidget();
    header->setObjectName("participantHeader");
    header->setFixedHeight(48);
    QHBoxLayout *headerLayout = new QHBoxLayout(header);
    headerLayout->setContentsMargins(16, 0, 16, 0);

    m_countLabel = new QLabel("参与者 (0)");
    m_countLabel->setObjectName("participantCountLabel");
    headerLayout->addWidget(m_countLabel);
    headerLayout->addStretch();

    mainLayout->addWidget(header);

    // Participant list
    m_participantListModel = new ParticipantListModel(this);
    m_participantListDelegate = new ParticipantListDelegate(this);

    m_participantListView = new QListView();
    m_participantListView->setObjectName("participantListView");
    m_participantListView->setModel(m_participantListModel);
    m_participantListView->setItemDelegate(m_participantListDelegate);
    m_participantListView->setContextMenuPolicy(Qt::CustomContextMenu);
    m_participantListView->setSpacing(4);

    mainLayout->addWidget(m_participantListView);
}

void ParticipantWidget::setupConnections()
{
    connect(m_participantListView, &QListView::clicked, this, &ParticipantWidget::onParticipantClicked);
    connect(m_participantListView, &QListView::customContextMenuRequested, this, &ParticipantWidget::onParticipantContextMenu);
    
    // Add sample participants for testing
    addSampleParticipants();
}

void ParticipantWidget::addSampleParticipants()
{
    // Create 3 sample participants
    User *user1 = new User("user001", "张三", this);
    user1->setStatus(UserStatus::Online);
    user1->setIsHost(true);
    addParticipant(user1);

    User *user2 = new User("user002", "李四", this);
    user2->setStatus(UserStatus::Online);
    user2->setIsMuted(true);
    addParticipant(user2);

    User *user3 = new User("user003", "王五", this);
    user3->setStatus(UserStatus::Online);
    user3->setIsCameraOff(true);
    addParticipant(user3);
}

void ParticipantWidget::onParticipantClicked(const QModelIndex &index)
{
    if (!index.isValid()) {
        return;
    }
    User *user = m_participantListModel->getParticipant(index.row());
    if (user) {
        emit participantClicked(user->id());
    }
}

void ParticipantWidget::onParticipantContextMenu(const QPoint &pos)
{
    QModelIndex index = m_participantListView->indexAt(pos);
    if (!index.isValid()) return;

    User *user = m_participantListModel->getParticipant(index.row());
    if (!user) return;
    
    QString userId = user->id();

    QMenu menu;

    QAction *muteAction = menu.addAction("静音");
    QAction *cameraAction = menu.addAction("关闭摄像头");
    menu.addSeparator();
    QAction *removeAction = menu.addAction("移除参与者");

    connect(muteAction, &QAction::triggered, this, [this, userId]() {
        emit participantMuteRequested(userId);
    });
    connect(cameraAction, &QAction::triggered, this, [this, userId]() {
        emit participantCameraRequested(userId);
    });
    connect(removeAction, &QAction::triggered, this, [this, userId]() {
        emit participantRemoveRequested(userId);
    });

    menu.exec(m_participantListView->mapToGlobal(pos));
}

void ParticipantWidget::updateParticipantItem(User *user)
{
    Q_UNUSED(user);
    // TODO: Update specific participant item
}
