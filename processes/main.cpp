#include "processor.h"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDir>

int main(int argc, char** argv) 
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    Processor processor(nullptr);

    engine.rootContext()->setContextProperty("processor", &processor);
    engine.load(QUrl::fromLocalFile(QDir::currentPath() + "/ui.qml"));

    return app.exec();
}