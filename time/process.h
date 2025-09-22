#ifndef PROCESS_H
#define PROCESS_H

#include <QObject>

struct Process : QObject
{
    Q_OBJECT

public:
    Process(QObject* parent, pid_t pid_id);
    ~Process() = default;

    Q_INVOKABLE void kill_process();

    Q_INVOKABLE void update_move_data(bool is_move);
    Q_INVOKABLE void update_hide_data(bool is_hide);
    Q_INVOKABLE void update_run_data(bool is_run);
    Q_INVOKABLE void update_x_pos_data(int x_pos);
    Q_INVOKABLE void update_y_pos_data(int y_pos);

    Q_INVOKABLE bool read_move_data();
    Q_INVOKABLE bool read_hide_data();
    Q_INVOKABLE int  read_x_pos_data();
    Q_INVOKABLE int  read_y_pos_data(); 

    pid_t pid_id;
};

#endif // PROCESS_H