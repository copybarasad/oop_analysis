#ifndef MOVE_COMMAND_H
#define MOVE_COMMAND_H

#include "command_interface.h"

class MoveCommand : public ICommand
{
private:
    int dx;
    int dy;

public:
    MoveCommand(int delta_x, int delta_y);

    CommandType get_type() const override;
    int get_dx() const;
    int get_dy() const;
};

#endif