#pragma once

#include <memory>
#include <string>

class NMEA {
public:
    NMEA(const std::string &port);
    ~NMEA();

    NMEA(const NMEA &) = delete;
    NMEA &operator=(const NMEA &) = delete;
    NMEA(NMEA &&) noexcept = default;
    NMEA &operator=(NMEA &&) noexcept = default;

    void run();

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;

    bool m_sending = false;
};
