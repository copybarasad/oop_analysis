#ifndef QUITCOMMAND_H
#define QUITCOMMAND_H

#include "Command.h"

class QuitCommand : public Command {
public:
    bool execute(GameState& gameState) override;
    std::string getName() const override;
};

#endif 