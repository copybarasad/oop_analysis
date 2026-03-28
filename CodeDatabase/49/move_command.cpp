#include "move_command.h"

MoveCommand::MoveCommand(int delta_x, int delta_y)
    : dx(delta_x), dy(delta_y) {}

CommandType MoveCommand::get_type() const
{
    return CommandType::MOVE;
}

int MoveCommand::get_dx() const
{
    return dx;
}

int MoveCommand::get_dy() const
{
    return dy;
}