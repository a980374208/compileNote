#ifndef PARTICIPANTLISTMODEL_H
#define PARTICIPANTLISTMODEL_H

#include <QAbstractListModel>
#include <QList>
#include "User.h"

/**
 * @brief 参与者列表数据模型
 * 基于 QAbstractListModel，提供参与者数据管理
 */
class ParticipantListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit ParticipantListModel(QObject *parent = nullptr);

    // Model interface
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Data management
    void addParticipant(User *user);
    void removeParticipant(const QString &userId);
    void removeParticipant(int row);
    void clear();
    void setParticipants(const QList<User*> &participants);
    void updateParticipant(const QString &userId);

    // Getters
    User* getParticipant(int row) const;
    User* getParticipantById(const QString &userId) const;
    QList<User*> getParticipants() const { return m_participants; }
    int getParticipantCount() const { return m_participants.count(); }

private:
    int findParticipantIndex(const QString &userId) const;

    QList<User*> m_participants;
};

#endif // PARTICIPANTLISTMODEL_H
