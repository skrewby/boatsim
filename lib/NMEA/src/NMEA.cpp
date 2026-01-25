#include "NMEA.hpp"
#include "N2kMessages.h"
#include "N2kTypes.h"
#include "NMEA2000.h"
#include "NMEA2000_SocketCAN.h"
#include <chrono>
#include <stdexcept>
#include <thread>
#include <type_traits>
#include <variant>

const unsigned long TransmitMessages[] PROGMEM = {129026L, 0};

struct NMEA::Impl {
    tNMEA2000_SocketCAN nmea;
    tSocketStream stream;

    Impl(const std::string &port) : nmea(const_cast<char *>(port.c_str())) {}
};

NMEA::NMEA(const std::string &port) : m_impl(std::make_unique<Impl>(port)) {
    m_impl->nmea.SetN2kCANSendFrameBufSize(250);
    m_impl->nmea.SetProductInformation(
        "00000001",             // Manufacturer's Model serial code
        2,                      // Manufacturer's product code
        "Boat Simulator",       // Manufacturer's Model ID
        "1.0.0.0 (2026-01-20)", // Manufacturer's Software version code
        "1.0.0.0 (2026-01-20)"  // Manufacturer's Model version
    );
    m_impl->nmea.SetDeviceInformation(
        1,   // Unique number. Use e.g. Serial number.
        132, // Device function=Analog to NMEA 2000 Gateway. See codes on
             // https://web.archive.org/web/20190531120557/https://www.nmea.org/Assets/20120726%20nmea%202000%20class%20&%20function%20codes%20v%202.00.pdf
        25,  // Device class=Inter/Intranetwork Device. See codes on
            // https://web.archive.org/web/20190531120557/https://www.nmea.org/Assets/20120726%20nmea%202000%20class%20&%20function%20codes%20v%202.00.pdf
        2046 // Just choosen free from code list on
             // https://web.archive.org/web/20190529161431/http://www.nmea.org/Assets/20121020%20nmea%202000%20registration%20list.pdf
    );
    m_impl->nmea.SetForwardStream(&m_impl->stream);    // PC output to default serial port
    m_impl->nmea.SetForwardType(tNMEA2000::fwdt_Text); // Show in clear text. Leave uncommented
                                                       // for default Actisense format.
    m_impl->nmea.SetForwardOwnMessages(false);         // Do not print own messages.
    //  We act as real node on bus. Some devices does not show messages, if they can not request
    //  information.
    m_impl->nmea.SetMode(tNMEA2000::N2km_ListenAndNode, 22);
    m_impl->nmea.EnableForward(true);
    m_impl->nmea.ExtendTransmitMessages(TransmitMessages);

    auto start = std::chrono::steady_clock::now();
    while (!m_impl->nmea.Open()) {
        auto elapsed = std::chrono::steady_clock::now() - start;
        if (elapsed > std::chrono::seconds(3)) {
            throw std::runtime_error("Failed to open CAN interface '" + port + "'");
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    RegisterMessages();
    m_running = true;
    m_thread = std::thread(&NMEA::Run, this);
}

NMEA::~NMEA() { Exit(); }

void NMEA::Start() {
    auto now = std::chrono::steady_clock::now();
    for (auto &msg : m_messages) {
        msg.last_send = now - msg.period + msg.offset;
    }
    m_sending = true;
}

void NMEA::Exit() {
    m_running = false;
    if (m_thread.joinable()) {
        m_thread.join();
    }
}

void NMEA::Stop() { m_sending = false; }

void NMEA::SetCOG(double value) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_state.cog = value;
}

void NMEA::SetSOG(double value) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_state.sog = value;
}

void NMEA::SetTemperature(double value) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_state.temperature = value;
}

void NMEA::SetHumidity(double value) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_state.humidity = value;
}

void NMEA::SetPressure(double value) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_state.pressure = value;
}

void NMEA::Run() {
    while (m_running) {
        if (m_sending) {
            SendMessages();
        }
        m_impl->nmea.ParseMessages();
    }
}

void NMEA::SendMessages() {
    State state;
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        state = m_state;
    }

    auto now = std::chrono::steady_clock::now();
    for (auto &msg : m_messages) {
        if (now - msg.last_send >= msg.period) {
            msg.last_send = now;
            SendMessage(msg.sender(state));
        }
    }
}

void NMEA::SendMessage(const Message &msg) {
    tN2kMsg nmea_msg;

    std::visit(
        [&](auto &&m) {
            using T = std::decay_t<decltype(m)>;

            if constexpr (std::is_same_v<T, Messages::COGSOGRapid>) {
                SetN2kCOGSOGRapid(nmea_msg, 1, N2khr_true, m.cog, m.sog);
            } else if constexpr (std::is_same_v<T, Messages::Temperature>) {
                SetN2kTemperature(nmea_msg, 1, 1, tN2kTempSource::N2kts_InsideTemperature,
                                  m.temperature);
            } else if constexpr (std::is_same_v<T, Messages::Humidity>) {
                SetN2kHumidity(nmea_msg, 1, 1, tN2kHumiditySource::N2khs_InsideHumidity,
                               m.humidity);
            } else if constexpr (std::is_same_v<T, Messages::Pressure>) {
                SetN2kSetPressure(nmea_msg, 1, 1, tN2kPressureSource::N2kps_Atmospheric,
                                  m.pressure);
            }
        },
        msg);

    m_impl->nmea.SendMsg(nmea_msg);
}

void NMEA::RegisterMessages() {
    using namespace std::chrono_literals;

    Register(250ms, 0ms, [](const State &s) { return Messages::COGSOGRapid{s.cog, s.sog}; });
    Register(2000ms, 40ms, [](const State &s) { return Messages::Temperature{s.temperature}; });
    Register(2000ms, 41ms, [](const State &s) { return Messages::Humidity{s.humidity}; });
    Register(2000ms, 42ms, [](const State &s) { return Messages::Pressure{s.pressure}; });
}

template <typename F>
void NMEA::Register(std::chrono::milliseconds period, std::chrono::milliseconds offset,
                    F &&sender) {
    using namespace std::chrono_literals;
    m_messages.push_back({period, offset + 300ms, {}, std::forward<F>(sender)});
}
