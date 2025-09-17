#ifndef PROCESS_H
#define PROCESS_H

#include <QObject>

struct Process : QObject
{
    Q_OBJECT

public:
    Process(QObject *parent, pid_t pid_id);
    ~Process() = default;

    Q_INVOKABLE void kill_process();

    pid_t pid_id;
};

#endif // PROCESS_H