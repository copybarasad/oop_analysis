#ifndef GAME_CASTSPELLCOMMAND_H
#define GAME_CASTSPELLCOMMAND_H

#include "ICommand.h"
#include "../../utils/Point.h"

namespace Game {

class CastSpellCommand : public ICommand {
public:
    CastSpellCommand(int spellIndex, Utils::Point target);
    void execute(Game& game) override;
private:
    int spellIndex;
    Utils::Point target;
};

}

#endif