#include "AgentInterface.h"

AgentInterface::AgentInterface(const QDBusConnection &bus, QObject *parent)
    : QObject(parent)
    , m_bus(bus) {
    
}

AgentInterface::~AgentInterface() = default;

void AgentInterface::Open(const Action &action, const QDBusMessage &message) {
    
}
