#include "process.h"

#include <iostream>
#include <cstdlib>
#include <string>

Process::Process(QObject *parent, pid_t pid_id) 
: QObject(parent), pid_id(pid_id) {}

void Process::kill_process() 
{
    std::string command = "kill " + std::to_string(static_cast<int>(pid_id));
    std::system(command.c_str());
}