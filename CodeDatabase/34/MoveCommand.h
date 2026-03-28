#ifndef GAME_MOVECOMMAND_H
#define GAME_MOVECOMMAND_H

#include "ICommand.h"

namespace Game {

class MoveCommand : public ICommand {
public:
    MoveCommand(int dr, int dc);
    void execute(Game& game) override;
private:
    int dr, dc;
};

}

#endif