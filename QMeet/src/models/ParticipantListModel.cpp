#include "ParticipantListModel.h"

ParticipantListModel::ParticipantListModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int ParticipantListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0;
    }
    return m_participants.count();
}

QVariant ParticipantListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_participants.count()) {
        return QVariant();
    }

    User *user = m_participants.at(index.row());

    switch (role) {
        case Qt::DisplayRole:
        case Qt::EditRole:
            return user->name();
        case Qt::UserRole:
            return user->id();
        case Qt::UserRole + 1:  // User pointer
            return QVariant::fromValue<void*>(static_cast<void*>(user));
        case Qt::UserRole + 2:  // Is muted
            return user->isMuted();
        case Qt::UserRole + 3:  // Is camera off
            return user->isCameraOff();
        case Qt::UserRole + 4:  // Is host
            return user->isHost();
        default:
            return QVariant();
    }
}

void ParticipantListModel::addParticipant(User *user)
{
    if (!user) {
        return;
    }

    beginInsertRows(QModelIndex(), m_participants.count(), m_participants.count());
    m_participants.append(user);
    endInsertRows();
}

void ParticipantListModel::removeParticipant(const QString &userId)
{
    int index = findParticipantIndex(userId);
    if (index >= 0) {
        removeParticipant(index);
    }
}

void ParticipantListModel::removeParticipant(int row)
{
    if (row < 0 || row >= m_participants.count()) {
        return;
    }

    beginRemoveRows(QModelIndex(), row, row);
    m_participants.removeAt(row);
    endRemoveRows();
}

void ParticipantListModel::clear()
{
    beginResetModel();
    m_participants.clear();
    endResetModel();
}

void ParticipantListModel::setParticipants(const QList<User*> &participants)
{
    beginResetModel();
    m_participants = participants;
    endResetModel();
}

void ParticipantListModel::updateParticipant(const QString &userId)
{
    int index = findParticipantIndex(userId);
    if (index >= 0) {
        QModelIndex modelIndex = this->index(index);
        emit dataChanged(modelIndex, modelIndex);
    }
}

User* ParticipantListModel::getParticipant(int row) const
{
    if (row < 0 || row >= m_participants.count()) {
        return nullptr;
    }
    return m_participants.at(row);
}

User* ParticipantListModel::getParticipantById(const QString &userId) const
{
    int index = findParticipantIndex(userId);
    if (index >= 0) {
        return m_participants.at(index);
    }
    return nullptr;
}

int ParticipantListModel::findParticipantIndex(const QString &userId) const
{
    for (int i = 0; i < m_participants.count(); ++i) {
        if (m_participants.at(i)->id() == userId) {
            return i;
        }
    }
    return -1;
}
