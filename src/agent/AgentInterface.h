#pragma once

#include <QObject>
#include <QDBusConnection>
#include <QDBusArgument>

#include "DBusTypes.h"

class AgentInterface : public QObject {
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.deepin.dde.dialog1.agent")

public:
    AgentInterface(const QDBusConnection &bus, QObject *parent = nullptr);
    ~AgentInterface();

public Q_SLOTS:
    Q_SCRIPTABLE void Open(const Action &action,const QDBusMessage &message);

private:
    QDBusConnection m_bus;
};
