#pragma once

#include <QObject>
#include <QDBusConnection>
#include <QDBusObjectPath>
#include <QDBusMessage>
#include <QFileSystemWatcher>
#include <QDBusArgument>

#include "Login1ManagerInterface.h"

class Dialog : public QObject {
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.deepin.dde.dialog1");

public:
    Dialog(const QDBusConnection &bus, QObject *parent = nullptr);
    ~Dialog();

public slots:
    Q_SCRIPTABLE void RegisterAgent(const QDBusMessage &message);
    Q_SCRIPTABLE void UnregisterAgent(const QDBusMessage &message);
    Q_SCRIPTABLE void Open(const QString &actionId, const QDBusMessage &message);

private:
    QString getSessionByService(const QString &service);

private slots:
    void UnregisterAgentByService(const QString &service);

private:
    QDBusConnection m_bus;
    std::vector<Action> m_actions;
    QHash<QString, QString> m_agents;

    org::freedesktop::login1::Manager m_login1Manager;
};
