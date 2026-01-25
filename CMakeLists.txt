cmake_minimum_required(VERSION 3.20)
project(boatsim)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
find_package(Qt6 REQUIRED COMPONENTS Quick Qml QuickControls2)
qt_standard_project_setup(REQUIRES 6.5)

set(PROJECT_SOURCES
    src/main.cpp
    src/BoatController.cpp
)

qt_add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
qt_add_qml_module(${PROJECT_NAME}
    URI BoatSim
    VERSION 1.0
    QML_FILES
        qml/Main.qml
)

add_subdirectory(lib)

target_include_directories(${PROJECT_NAME} PRIVATE "src")
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Quick Qt6::Qml Qt6::QuickControls2 NMEA)

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_BINARY_DIR}/compile_commands.json
        ${CMAKE_SOURCE_DIR}/compile_commands.json
    COMMENT "Creating compile_commands.json"
)
