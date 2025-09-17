#include "process.h"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDir>
#include <QProcess>

#include <functional>
#include <unistd.h>
#include <csignal>
#include <iostream>

void signal_handler(int signum) 
{
    if (signum == SIGTERM)  QCoreApplication::exit(0);
}

int main(int argc, char** argv) 
{
    pid_t pid = fork();
    
    if (pid < 0) return EXIT_FAILURE;
    if (pid > 0) return EXIT_SUCCESS;
    
    std::signal(SIGTERM, signal_handler);

    if (setsid() < 0) return EXIT_FAILURE;

    std::cout << "Daemon (" + std::string(argv[0]) + ") is running with PID " << getpid() << std::endl;
    
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    Process process_helper(nullptr, getpid());
    
    engine.rootContext()->setContextProperty("process_helper", &process_helper);
    engine.load(QUrl::fromLocalFile(QDir::currentPath() + "/ui.qml"));

    return app.exec();
}