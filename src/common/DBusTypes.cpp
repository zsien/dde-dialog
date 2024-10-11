#include "DBusTypes.h"

QDBusArgument &operator<<(QDBusArgument &argument, const ListSessionsSession &listSessionSession)
{
    argument.beginStructure();
    argument << listSessionSession.id;
    argument << listSessionSession.userId;
    argument << listSessionSession.username;
    argument << listSessionSession.seatId;
    argument << listSessionSession.objectPath;
    argument.endStructure();

    return argument;
}

const QDBusArgument &operator>>(const QDBusArgument &argument, ListSessionsSession &listSessionSession)
{
    argument.beginStructure();
    argument >> listSessionSession.id;
    argument >> listSessionSession.userId;
    argument >> listSessionSession.username;
    argument >> listSessionSession.seatId;
    argument >> listSessionSession.objectPath;
    argument.endStructure();

    return argument;
}


QDBusArgument &operator<<(QDBusArgument &argument, const Action &action)
{
    argument.beginStructure();
    argument << action.id;
    argument << action.gettextDomain;
    argument << action.type;
    argument << action.description;
    argument << action.message;
    argument.endStructure();

    return argument;
}

const QDBusArgument &operator>>(const QDBusArgument &argument, Action &action)
{
    argument.beginStructure();
    argument >> action.id;
    argument >> action.gettextDomain;
    argument >> action.type;
    argument >> action.description;
    argument >> action.message;
    argument.endStructure();

    return argument;
}
