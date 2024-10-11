#pragma once

#include <QString>
#include <QDBusArgument>
#include <QDBusObjectPath>

struct ListSessionsSession {
    QString id;
    uint32_t userId;
    QString username;
    QString seatId;
    QDBusObjectPath objectPath;
};
Q_DECLARE_METATYPE(ListSessionsSession)

QDBusArgument &operator<<(QDBusArgument &argument, const ListSessionsSession &listSessionSession);
const QDBusArgument &operator>>(const QDBusArgument &argument, ListSessionsSession &listSessionSession);

struct Action {
    QString id;
    QString gettextDomain;
    QString type;
    QString description;
    QString message;
};
Q_DECLARE_METATYPE(Action)

QDBusArgument &operator<<(QDBusArgument &argument, const Action &action);
const QDBusArgument &operator>>(const QDBusArgument &argument, Action &action);