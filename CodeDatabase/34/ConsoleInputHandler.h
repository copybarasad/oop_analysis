#ifndef GAME_CONSOLEINPUTHANDLER_H
#define GAME_CONSOLEINPUTHANDLER_H

#include "IInputHandler.h"
#include "KeyConfig.h"

namespace Game {

class Game;

class ConsoleInputHandler : public IInputHandler {
public:
    // Конструктор принимает и игру, и готовую конфигурацию
    ConsoleInputHandler(const Game& game, const KeyConfig& config);
    std::unique_ptr<ICommand> getCommand() override;

private:
    const Game& gameInstance;
    const KeyConfig& keyConfig; 

    void displayPlayerSpells() const;
};

}

#endif