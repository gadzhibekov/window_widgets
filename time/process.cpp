#include "process.h"
#include "../config.h"

#include <iostream>
#include <cstdlib>
#include <string>

Process::Process(QObject *parent, pid_t pid_id) 
: QObject(parent), pid_id(pid_id) {}

void Process::kill_process() 
{
    std::system(std::string((std::string)"kill " + std::to_string(static_cast<int>(pid_id))).c_str());
}

void Process::update_move_data(bool is_move)
{
    ConfigData config_data = Config::read();
    Config::write(ConfigData({is_move, config_data.is_hide, config_data.is_run, config_data.x, config_data.y}));
}

void Process::update_hide_data(bool is_hide)
{
    ConfigData config_data = Config::read();
    Config::write(ConfigData({config_data.is_move, is_hide, config_data.is_run, config_data.x, config_data.y}));
}

void Process::update_run_data(bool is_run)
{
    ConfigData config_data = Config::read();
    Config::write(ConfigData({config_data.is_move, config_data.is_hide, is_run, config_data.x, config_data.y}));
}

void Process::update_x_pos_data(int x_pos)
{
    ConfigData config_data = Config::read();
    Config::write(ConfigData({config_data.is_move, config_data.is_hide, config_data.is_run, x_pos, config_data.y}));
}

void Process::update_y_pos_data(int y_pos)
{
    ConfigData config_data = Config::read();
    Config::write(ConfigData({config_data.is_move, config_data.is_hide, config_data.is_run, config_data.x, y_pos}));
}

bool Process::read_move_data()   { return Config::read().is_move; }
bool Process::read_hide_data()   { return Config::read().is_hide; }
int  Process::read_x_pos_data()  { return Config::read().x;       }
int  Process::read_y_pos_data()  { return Config::read().y;       }