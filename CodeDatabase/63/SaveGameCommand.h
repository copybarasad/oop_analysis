#ifndef RPG_SAVEGAMECOMMAND_H
#define RPG_SAVEGAMECOMMAND_H

#include "Command.h"
#include "../JsonSaveHandler.h"

class SaveGameCommand : public Command {
    std::shared_ptr<Game>& game;
    bool& changeMode;
public:
    SaveGameCommand(std::shared_ptr<Game>& game, bool& changeMode) : game(game), changeMode(changeMode) {}

    std::pair<bool, std::string> execute() override {
        std::string message;
        try {
            JsonSaveHandler file("save.json");
            json data = game->to_json();
            file.save(data);
            message = "Игра сохранена!\n";
        } catch (std::exception& error) {
            message = std::string(error.what()) + "\nНе удалось сохранить игру\n";
        }
        changeMode = true;
        return {false, message + "Нажмите любую кнопку для продолжения\n"};
    };
};

#endif