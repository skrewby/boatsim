#include "BoatController.hpp"

QStringList BoatController::CanInterfaces() const {
    QStringList interfaces;
    QDir netDir("/sys/class/net");
    for (const QString &iface : netDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        QFile typeFile(QString("/sys/class/net/%1/type").arg(iface));
        if (typeFile.open(QIODevice::ReadOnly)) {
            if (typeFile.readAll().trimmed() == "280") {
                interfaces << iface;
            }
        }
    }
    return interfaces;
}

bool BoatController::ConnectToInterface(const QString &interface) {
    m_errorMessage.clear();
    emit ErrorMessageChanged();
    try {
        m_nmea = std::make_unique<NMEA>(interface.toStdString());
        emit ConnectedChanged();
        return true;
    } catch (const std::exception &e) {
        m_errorMessage = QString::fromStdString(e.what());
        emit ErrorMessageChanged();
        return false;
    }
}

void BoatController::Start() {
    if (m_nmea) {
        m_nmea->Start();
        emit SendingChanged();
    }
}

void BoatController::Stop() {
    if (m_nmea) {
        m_nmea->Stop();
        emit SendingChanged();
    }
}

void BoatController::SetCog(double value) {
    if (m_cog != value) {
        m_cog = value;
        if (m_nmea) {
            m_nmea->SetCOG(value);
        }
        emit CogChanged();
    }
}

void BoatController::SetSog(double value) {
    if (m_sog != value) {
        m_sog = value;
        if (m_nmea) {
            m_nmea->SetSOG(value);
        }
        emit SogChanged();
    }
}

void BoatController::SetTemperature(double value) {
    if (m_temperature != value) {
        m_temperature = value;
        if (m_nmea) {
            m_nmea->SetTemperature(value);
        }
        emit TemperatureChanged();
    }
}

void BoatController::SetHumidity(double value) {
    if (m_humidity != value) {
        m_humidity = value;
        if (m_nmea) {
            m_nmea->SetHumidity(value);
        }
        emit HumidityChanged();
    }
}

void BoatController::SetPressure(double value) {
    if (m_pressure != value) {
        m_pressure = value;
        if (m_nmea) {
            m_nmea->SetPressure(value);
        }
        emit PressureChanged();
    }
}
