#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "PlayerViewModel.h"

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    PlayerViewModel vm;
    engine.rootContext()->setContextProperty("vm", &vm);
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection
        );
    engine.loadFromModule("mediaplayer_lab9", "Main");
    return app.exec();
}