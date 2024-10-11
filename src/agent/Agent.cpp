#include "Agent.h"

const QString DIALOG_SERVICE = "org.deepin.dde.dialog1";
const QString DIALOG_PATH = "/org/deepin/dde/dialog1";

Agent::Agent()
    : m_bus(QDBusConnection::systemBus())
    , m_interface(m_bus)
    , m_dialog(DIALOG_SERVICE, DIALOG_PATH, m_bus) {
    if (!m_bus.registerObject("/org/deepin/dde/dialog1/agent", &m_interface, QDBusConnection::ExportScriptableContents)) {
        throw std::runtime_error("failed to register object");
    }

    auto reply = m_dialog.RegisterAgent();
    reply.waitForFinished();
    if (reply.isError()) {
        throw std::runtime_error("failed to register agent: " + reply.error().message().toStdString());
    }
}

Agent::~Agent() = default;
