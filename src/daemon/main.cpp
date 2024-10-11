#include "Dialog.h"

#include <QCoreApplication>
#include <QDBusConnection>


int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    auto bus = QDBusConnection::systemBus();
    if (!bus.registerService("org.deepin.dde.dialog1")) {
        qFatal("Failed to register service");
        return 1;
    }

    if (!bus.registerObject("/org/deepin/dde/dialog1", new Dialog(bus), QDBusConnection::ExportScriptableContents)) {
        qFatal("Failed to register object");
        return 1;
    }

    return a.exec();
}
