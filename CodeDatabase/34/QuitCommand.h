#ifndef GAME_QUITCOMMAND_H
#define GAME_QUITCOMMAND_H

#include "ICommand.h"

namespace Game {

class QuitCommand : public ICommand {
public:
    void execute(Game& game) override;
};

}

#endif
