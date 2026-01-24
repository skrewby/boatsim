#include "BoatController.hpp"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    BoatController boatController;
    engine.rootContext()->setContextProperty("boatController", &boatController);

    engine.loadFromModule("BoatSim", "Main");

    if (engine.rootObjects().isEmpty()) {
        return -1;
    }

    return app.exec();
}
