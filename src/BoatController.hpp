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
    Q_PROPERTY(double cog READ Cog WRITE SetCog NOTIFY CogChanged)
    Q_PROPERTY(double sog READ Sog WRITE SetSog NOTIFY SogChanged)
    Q_PROPERTY(double temperature READ Temperature WRITE SetTemperature NOTIFY TemperatureChanged)
    Q_PROPERTY(double humidity READ Humidity WRITE SetHumidity NOTIFY HumidityChanged)
    Q_PROPERTY(double pressure READ Pressure WRITE SetPressure NOTIFY PressureChanged)

public:
    explicit BoatController(QObject *parent = nullptr) : QObject(parent) {}

    QString ErrorMessage() const { return m_errorMessage; }

    QStringList CanInterfaces() const;

    bool Connected() const { return m_nmea != nullptr; }

    bool Sending() const { return m_nmea && m_nmea->IsSending(); }

    Q_INVOKABLE bool ConnectToInterface(const QString &interface);

    Q_INVOKABLE void Start();

    Q_INVOKABLE void Stop();

    double Cog() const { return m_cog; }

    void SetCog(double value);

    double Sog() const { return m_sog; }

    void SetSog(double value);

    double Temperature() const { return m_temperature; }

    void SetTemperature(double value);

    double Humidity() const { return m_humidity; }

    void SetHumidity(double value);

    double Pressure() const { return m_pressure; }

    void SetPressure(double value);

signals:
    void CanInterfacesChanged();
    void ConnectedChanged();
    void ErrorMessageChanged();
    void SendingChanged();
    void CogChanged();
    void SogChanged();
    void TemperatureChanged();
    void HumidityChanged();
    void PressureChanged();

private:
    std::unique_ptr<NMEA> m_nmea;
    QString m_errorMessage;
    double m_cog{0.0};
    double m_sog{0.0};
    double m_temperature{0.0};
    double m_humidity{0.0};
    double m_pressure{0.0};
};
