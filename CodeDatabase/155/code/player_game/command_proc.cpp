#include <string>
#include <iostream>
#include "command_proc.h"

command_proc::command_proc(){}

Command* command_proc::get_command()
{
    Command* comm = new Command();
    comm->int_comm = -1;
    bool notdigit;
    std::cin >> comm->str_comm;
    for (char i : comm->str_comm)
    {
        if (!isdigit(i))
        {
            notdigit = true;
            break;
        }
    }
    if (notdigit)
    {
        comm->int_comm = -1;
    }
    else
    {
        comm->int_comm = stoi(comm->str_comm.substr(0, 5));
    }
    return comm;
}

command_proc::~command_proc(){}
