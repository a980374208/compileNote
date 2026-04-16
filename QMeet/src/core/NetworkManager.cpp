#include "NetworkManager.h"
#include <QJsonDocument>
#include <QJsonValue>

NetworkManager::NetworkManager(QObject *parent)
    : QObject(parent)
    , m_webSocket(new QWebSocket())
{
    connect(m_webSocket, &QWebSocket::connected, this, &NetworkManager::onWebSocketConnected);
    connect(m_webSocket, &QWebSocket::disconnected, this, &NetworkManager::onWebSocketDisconnected);
    connect(m_webSocket, &QWebSocket::textMessageReceived, this, &NetworkManager::onWebSocketMessageReceived);
    connect(m_webSocket, &QWebSocket::errorOccurred, this, &NetworkManager::onWebSocketError);
}

NetworkManager::~NetworkManager()
{
    disconnectFromServer();
    delete m_webSocket;
}

void NetworkManager::connectToServer(const QUrl &serverUrl)
{
    m_webSocket->open(serverUrl);
}

void NetworkManager::disconnectFromServer()
{
    if (m_webSocket->state() == QAbstractSocket::ConnectedState) {
        m_webSocket->close();
    }
}

bool NetworkManager::isConnected() const
{
    return m_webSocket->state() == QAbstractSocket::ConnectedState;
}

void NetworkManager::sendRequest(const QString &type, const QJsonObject &data)
{
    if (!isConnected()) {
        emit error("Not connected to server");
        return;
    }

    QJsonObject message;
    message["type"] = type;
    message["data"] = data;
    message["timestamp"] = QDateTime::currentDateTime().toMSecsSinceEpoch();

    QJsonDocument doc(message);
    m_webSocket->sendTextMessage(doc.toJson(QJsonDocument::Compact));
}

void NetworkManager::sendAuthRequest(const QString &token)
{
    m_token = token;
    QJsonObject data;
    data["token"] = token;
    sendRequest("auth", data);
}

void NetworkManager::onWebSocketConnected()
{
    emit connected();
}

void NetworkManager::onWebSocketDisconnected()
{
    m_authenticated = false;
    emit disconnected();
}

void NetworkManager::onWebSocketMessageReceived(const QString &message)
{
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8(), &error);

    if (error.error != QJsonParseError::NoError) {
        emit this->error("Failed to parse message: " + error.errorString());
        return;
    }

    handleMessage(doc.object());
}

void NetworkManager::onWebSocketError(QAbstractSocket::SocketError error)
{
    Q_UNUSED(error);
    emit this->error(m_webSocket->errorString());
}

void NetworkManager::handleMessage(const QJsonObject &message)
{
    QString type = message["type"].toString();
    QJsonObject data = message["data"].toObject();

    if (type == "auth_success") {
        m_authenticated = true;
        emit authenticated();
    } else if (type == "auth_failed") {
        emit authenticationFailed(data["reason"].toString());
    } else {
        emit messageReceived(type, data);
    }
}
