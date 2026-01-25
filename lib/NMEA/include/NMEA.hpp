#pragma once

#include <atomic>
#include <memory>
#include <string>
#include <thread>

class NMEA {
public:
    NMEA(const std::string &port);
    ~NMEA();

    NMEA(const NMEA &) = delete;
    NMEA &operator=(const NMEA &) = delete;
    NMEA(NMEA &&) noexcept = delete;
    NMEA &operator=(NMEA &&) noexcept = delete;

    void Start();
    void Stop();
    void Exit();

    bool IsRunning() const { return m_running; }

private:
    void Run();

    struct Impl;
    std::unique_ptr<Impl> m_impl;

    std::atomic<bool> m_running{false};
    std::atomic<bool> m_sending{false};
    std::thread m_thread;
};
