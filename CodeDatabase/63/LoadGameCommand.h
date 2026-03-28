#ifndef RPG_LOADGAMECOMMAND_H
#define RPG_LOADGAMECOMMAND_H

#include <memory>
#include "Command.h"
class Game;

class LoadGameCommand : public Command {
    std::shared_ptr<Game>& game;
    bool& modeChanged;
public:
    LoadGameCommand(std::shared_ptr<Game>& game, bool& modeChanged): game(game), modeChanged(modeChanged) {}

    std::pair<bool, std::string> execute() override {
        std::string message;
        try {
            message = "Игра успешно загружена\n";
            JsonLoadHandler file("save.json");
            json data = file.load();
            game = std::make_shared<Game>(data);
            // modeChanged = true;
        } catch (std::runtime_error& error) {
            message = std::string(error.what()) + "\nНе удалось загрузить сохранение\n";
        }
        modeChanged = true;
        return {false, message + "Нажмите любую клавишу для продолжения\n"};
    }
};

#endif