#ifndef RPG_NEWGAMECOMMAND_H
#define RPG_NEWGAMECOMMAND_H

#include <memory>
#include "Command.h"
#include "../Game.h"

class NewGameCommand : public Command {
    std::shared_ptr<Game>& game;
    bool& modeChanged;
public:
    NewGameCommand(std::shared_ptr<Game>& game, bool& modeChanged) : game(game), modeChanged(modeChanged) {}

    std::pair<bool, std::string> execute() override {
        game = std::make_shared<Game>();
        modeChanged = true;
        return {false, "Начинаем новую игру.\nНажмите любую клавишу для продолжения...\n"};
    }
};

#endif