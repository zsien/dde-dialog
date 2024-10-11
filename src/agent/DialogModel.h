#pragma once

#include <QAbstractItemModel>
#include <QQmlEngine>

class DialogModel : public QAbstractItemModel {
    Q_OBJECT
    QML_ELEMENT

public:
    DialogModel(QObject *parent = nullptr);
    ~DialogModel();
};
