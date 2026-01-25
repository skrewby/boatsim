#pragma once

#include "State.hpp"
#include <atomic>
#include <functional>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <variant>

namespace Messages {
struct COGSOGRapid {
    double cog;
    double sog;
};

struct Temperature {
    double temperature;
};

struct Humidity {
    double humidity;
};

struct Pressure {
    double pressure;
};
} // namespace Messages

using Message = std::variant<Messages::COGSOGRapid, Messages::Temperature, Messages::Humidity,
                             Messages::Pressure>;

struct MessageConfig {
    std::chrono::milliseconds period;
    std::chrono::milliseconds offset;
    std::chrono::steady_clock::time_point last_send{};
    std::function<Message(const State &)> sender;
};

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

    bool IsSending() const { return m_sending; }

private:
    void Run();
    void SendMessages();
    void SendMessage(const Message &msg);
    void RegisterMessages();
    template <typename F>
    void Register(std::chrono::milliseconds period, std::chrono::milliseconds offset, F &&sender);

    struct Impl;
    std::unique_ptr<Impl> m_impl;

    std::atomic<bool> m_running{false};
    std::atomic<bool> m_sending{false};
    std::thread m_thread;

    mutable std::mutex m_mutex;
    State m_state;
    std::vector<MessageConfig> m_messages;
};
