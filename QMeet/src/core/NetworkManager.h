#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>
#include <QWebSocket>
#include <QUrl>
#include <QJsonObject>

/**
 * @brief 网络管理器
 * 负责与服务器的 WebSocket 通信
 */
class NetworkManager : public QObject
{
    Q_OBJECT

public:
    explicit NetworkManager(QObject *parent = nullptr);
    ~NetworkManager() override;

    void connectToServer(const QUrl &serverUrl);
    void disconnectFromServer();
    bool isConnected() const;

    void sendRequest(const QString &type, const QJsonObject &data);
    void sendAuthRequest(const QString &token);

signals:
    void connected();
    void disconnected();
    void error(const QString &message);

    void messageReceived(const QString &type, const QJsonObject &data);
    void authenticated();
    void authenticationFailed(const QString &reason);

private slots:
    void onWebSocketConnected();
    void onWebSocketDisconnected();
    void onWebSocketMessageReceived(const QString &message);
    void onWebSocketError(QAbstractSocket::SocketError error);

private:
    void handleMessage(const QJsonObject &message);

    QWebSocket *m_webSocket = nullptr;
    bool m_authenticated = false;
    QString m_token;
};

#endif // NETWORKMANAGER_H
