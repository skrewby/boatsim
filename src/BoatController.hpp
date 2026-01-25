#pragma once

#include "NMEA.hpp"
#include <QDir>
#include <QFile>
#include <QObject>
#include <QStringList>
#include <memory>

class BoatController : public QObject {
    Q_OBJECT
    Q_PROPERTY(QStringList CanInterfaces READ CanInterfaces NOTIFY CanInterfacesChanged)
    Q_PROPERTY(bool Connected READ Connected NOTIFY ConnectedChanged)
    Q_PROPERTY(QString ErrorMessage READ ErrorMessage NOTIFY ErrorMessageChanged)

public:
    explicit BoatController(QObject *parent = nullptr) : QObject(parent) {}

    QString ErrorMessage() const { return m_errorMessage; }

    QStringList CanInterfaces() const;

    bool Connected() const { return m_nmea != nullptr; }

    Q_INVOKABLE bool ConnectToInterface(const QString &interface);

    Q_INVOKABLE void RunNMEA();

signals:
    void CanInterfacesChanged();
    void ConnectedChanged();
    void ErrorMessageChanged();

private:
    std::unique_ptr<NMEA> m_nmea;
    QString m_errorMessage;
};
