#include "Dialog.h"

#include <QDBusConnectionInterface>

#include <yaml-cpp/yaml.h>

#include <systemd/sd-login.h>

#include <filesystem>

#include "config.h"
#include "Login1SessionInterface.h"
#include "AgentInterface.h"

const QString LOGIN1_SERVICE = "org.freedesktop.login1";
const QString LOGIN1_MANAGER_PATH = "/org/freedesktop/login1";

const QString AGENT_PATH = "/org/deepin/dde/dialog1/agent";

struct Data {
    std::vector<Action> actions;
};

namespace YAML {
template<>
struct convert<Data> {
  static bool decode(const Node &node, Data &rhs) {
    if(!node.IsMap()) {
        return false;
    }

    if (!node["actions"]) {
        return false;
    }

    for (auto it = node["actions"].begin(); it != node["actions"].end(); ++it) {
        rhs.actions.push_back({
            QString::fromStdString((*it)["id"].as<std::string>()),
            QString::fromStdString((*it)["gettext-domain"].as<std::string>()),
            QString::fromStdString((*it)["type"].as<std::string>()),
            QString::fromStdString((*it)["description"].as<std::string>()),
            QString::fromStdString((*it)["message"].as<std::string>()),
        });
    }

    return true;
  }
};
}

Dialog::Dialog(const QDBusConnection &bus, QObject *parent)
    : QObject(parent)
    , m_bus(bus)
    , m_login1Manager(LOGIN1_SERVICE, LOGIN1_MANAGER_PATH, m_bus) {
    connect(m_bus.interface(), &QDBusConnectionInterface::serviceUnregistered, this, &Dialog::UnregisterAgentByService);

    std::filesystem::directory_iterator it(DATA_DIR);
    for (auto const &entry : std::filesystem::directory_iterator{DATA_DIR}) {
        if (!entry.is_regular_file()) {
            continue;
        }

        if (entry.path().extension() != ".yaml") {
            continue;
        }

        try {
            auto doc = YAML::LoadFile(entry.path()).as<Data>();
            m_actions.insert(m_actions.end(), doc.actions.begin(), doc.actions.end());
        } catch (const YAML::ParserException &e) {
            // todo: log
        } catch (const YAML::BadFile &e) {
            // todo: log
        } catch (const YAML::InvalidNode &e) {
            // todo: log
        }
    }
}

Dialog::~Dialog() = default;

void Dialog::RegisterAgent(const QDBusMessage &message) {
    QString session = getSessionByService(message.service());
    if (session.isEmpty()) {
        // todo: log
        message.createErrorReply(QDBusError::Failed, "No session found");
        return;
    }

    if (m_agents.contains(session)) {
        message.createErrorReply(QDBusError::AccessDenied, "Agent already registered");
        return;
    }

    m_agents[session] = message.service();
}

void Dialog::UnregisterAgent(const QDBusMessage &message) {
    UnregisterAgentByService(message.service());
}

void Dialog::Open(const QString &actionId, const QDBusMessage &message) {
    auto it = std::find_if(m_actions.begin(), m_actions.end(), [&actionId](const auto &action) {
        return action.id == actionId;
    });

    if (it == m_actions.end()) {
        message.createErrorReply(QDBusError::Failed, "Action not found");
        return;
    }

    auto reply = m_login1Manager.ListSessions();
    reply.waitForFinished();
    if (reply.isError()) {
        message.createErrorReply(QDBusError::Failed, "Failed to get sessions");
        return;
    }

    QString activatedSession;
    auto sessions = reply.value();
    for (auto &session : sessions) {
        if (!m_agents.contains(session.id)) {
            continue;
        }

        org::freedesktop::login1::Session s(LOGIN1_SERVICE, session.objectPath.path(), m_bus);
        if (s.active()) {
            activatedSession = session.id;
            break;
        }
    }

    if (activatedSession.isEmpty()) {
        message.createErrorReply(QDBusError::Failed, "No activated agent");
        return;
    }

    auto agentService = m_agents[activatedSession];
    org::deepin::dde::dialog1::agent agent(agentService, AGENT_PATH, m_bus);
    agent.Open(*it);
}

QString Dialog::getSessionByService(const QString &service) {
    auto pid = m_bus.interface()->servicePid(service);
    if (!pid.isValid()) {
        // todo: log
        return "";
    }

    char *session = nullptr;
    int res = sd_pid_get_session(pid, &session);
    if (res >= 0) {
        QString result = QString::fromUtf8(session);
        free(session);
        return result;
    }

    uid_t uid = 0;
    res = sd_pid_get_owner_uid(pid, &uid);
    if (res >= 0) {
        res = sd_uid_get_display(uid, &session);
        if (res >= 0) {
            QString result = QString::fromUtf8(session);
            free(session);
            return result;
        }
    }

    return "";
}

void Dialog::UnregisterAgentByService(const QString &service) {
    m_agents.removeIf([&service](const auto &pair) { return pair.value() == service; });
}
