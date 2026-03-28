#ifndef CASTSPELLCOMMAND_H
#define CASTSPELLCOMMAND_H

#include "Command.h"

class InputHandler;
class GameState;

class CastSpellCommand : public Command {
private:
    InputHandler& inputHandler;

public:
    CastSpellCommand(InputHandler& handler);
    bool execute(GameState& gameState) override;
    std::string getName() const override;
};

#endif 