add_library(nmea2000 STATIC
    vendor/NMEA2000/src/N2kMsg.cpp
    vendor/NMEA2000/src/N2kStream.cpp
    vendor/NMEA2000/src/N2kMessages.cpp
    vendor/NMEA2000/src/N2kTimer.cpp
    vendor/NMEA2000/src/Seasmart.cpp
    vendor/NMEA2000/src/N2kDeviceList.cpp
    vendor/NMEA2000/src/N2kGroupFunction.cpp
    vendor/NMEA2000/src/N2kGroupFunctionDefaultHandlers.cpp
    vendor/NMEA2000/src/N2kMaretron.cpp
    vendor/NMEA2000/src/N2kCZone.cpp
    vendor/NMEA2000/src/NMEA2000.cpp
)
target_include_directories(nmea2000 PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/NMEA2000/src
)

add_library(nmea2000_socketcan
    vendor/NMEA2000_socketCAN/NMEA2000_SocketCAN.cpp
)
target_include_directories(nmea2000_socketcan PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/NMEA2000_socketCAN
)
target_link_libraries(nmea2000_socketcan PUBLIC nmea2000)

set(LIB_SOURCES
    src/NMEA.cpp
)

add_library(NMEA STATIC
    ${LIB_SOURCES}
)

target_include_directories(NMEA PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_include_directories(NMEA PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(NMEA PRIVATE nmea2000 nmea2000_socketcan)
