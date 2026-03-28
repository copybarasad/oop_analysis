#ifndef RPG_LOADFROMGAMECOMMAND_H
#define RPG_LOADFROMGAMECOMMAND_H

#include "Command.h"
#include "../Game.h"

class LoadFromGameCommand : public Command {
    std::shared_ptr<Game>& game;
    bool& changeMode;
public:
    LoadFromGameCommand(std::shared_ptr<Game>& game, bool& changeMode) : game(game), changeMode(changeMode) {}

    std::pair<bool, std::string> execute() override {
        std::string message;
        try {
            JsonLoadHandler file("save.json");
            json data = file.load();
            game = std::make_shared<Game>(data);
            message = "Игра успешно загружена\n";
        } catch (std::exception& error) {
            message = std::string(error.what()) + "\nНе удалось загрузить игру из сохранения, продолжаем текущую\n";
        }
        changeMode = true;
        return {false, message + "Нажмите любую кнопку для продолжения\n"};
    };
};


#endif