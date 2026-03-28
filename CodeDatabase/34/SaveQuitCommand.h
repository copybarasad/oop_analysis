#ifndef GAME_SAVEQUITCOMMAND_H
#define GAME_SAVEQUITCOMMAND_H

#include "ICommand.h"

namespace Game {

class SaveQuitCommand : public ICommand {
public:
    void execute(Game& game) override;
};

}

#endif
