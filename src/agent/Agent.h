#pragma once

#include "QObject"
#include <QQmlEngine>
#include <QDBusConnection>

#include "AgentInterface.h"
#include "DialogInterface.h"

class Agent : public QObject {
    Q_OBJECT
    QML_ELEMENT

public:
    Agent();
    ~Agent();

private:
    QDBusConnection m_bus;
    AgentInterface m_interface;
    org::deepin::dde::dialog1 m_dialog;
};
