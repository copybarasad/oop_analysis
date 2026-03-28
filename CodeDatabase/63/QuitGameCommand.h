#ifndef RPG_QUITGAMECOMMAND_H
#define RPG_QUITGAMECOMMAND_H

#include "Command.h"
#include "../Game.h"
#include "../JsonSaveHandler.h"

class QuitGameCommand : public Command {
    std::shared_ptr<Game>& game;
    bool& changeMode;
public:
    QuitGameCommand(std::shared_ptr<Game>& game, bool& changeMode) : game(game), changeMode(changeMode) {}

    std::pair<bool, std::string> execute() override {
        std::string message;
        try {
            JsonSaveHandler file("save.json");
            json data = game->to_json();
            file.save(data);
            message = "Игра сохранена, выход в меню...\n";
        } catch (std::exception& error) {
            message = std::string(error.what()) + "\nНе удалось сохранить игру, выход в меню\n";
        }
        game = nullptr;
        changeMode = true;
        return {false, message + "Нажмите любую клавишу для возврата в меню\n"};
    }
};

#endif