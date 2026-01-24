#pragma once

#include "NMEA.hpp"
#include <QObject>

class BoatController : public QObject {
    Q_OBJECT

public:
    explicit BoatController(QObject *parent = nullptr) : QObject(parent) {}

    Q_INVOKABLE void runNMEA() { m_nmea.run(); }

private:
    NMEA m_nmea;
};
