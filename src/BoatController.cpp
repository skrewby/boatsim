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

void BoatController::RunNMEA() {
    if (m_nmea) {
        m_nmea->Start();
    }
}
