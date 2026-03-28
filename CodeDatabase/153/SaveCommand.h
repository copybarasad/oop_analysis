#ifndef SAVECOMMAND_H
#define SAVECOMMAND_H

#include "Command.h"

class GameSaveSystem;
class GameState;

class SaveCommand : public Command {
private:
    GameSaveSystem& saveSystem;

public:
    SaveCommand(GameSaveSystem& save);
    bool execute(GameState& gameState) override;
    std::string getName() const override;
};

#endif