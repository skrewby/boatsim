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
    Q_PROPERTY(bool Sending READ Sending NOTIFY SendingChanged)

public:
    explicit BoatController(QObject *parent = nullptr) : QObject(parent) {}

    QString ErrorMessage() const { return m_errorMessage; }

    QStringList CanInterfaces() const;

    bool Connected() const { return m_nmea != nullptr; }

    bool Sending() const { return m_nmea && m_nmea->IsSending(); }

    Q_INVOKABLE bool ConnectToInterface(const QString &interface);

    Q_INVOKABLE void Start();

    Q_INVOKABLE void Stop();

signals:
    void CanInterfacesChanged();
    void ConnectedChanged();
    void ErrorMessageChanged();
    void SendingChanged();

private:
    std::unique_ptr<NMEA> m_nmea;
    QString m_errorMessage;
};
