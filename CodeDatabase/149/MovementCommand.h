#ifndef MOVEMENTCOMMAND_H
#define MOVEMENTCOMMAND_H

#include "Command.h"

class MovementCommand : public Command {
private:
    CommandType type;
    int deltaX;
    int deltaY;

public:
    MovementCommand(CommandType commandType, int dx, int dy);
    CommandType getType() const override;
    bool isValid() const override;
    int getDeltaX() const;
    int getDeltaY() const;
};

#endif
