#pragma once
#ifndef GAMECOMMAND_H
#define GAMECOMMAND_H

#include "ICommand.h"
#include <string>
#include <functional>

class Player;
class Map;
class Game;

class GameCommand : public ICommand {
private:
    std::string name;
    std::string description;
    std::function<bool(Player&, Map&, Game&)> action;

public:
    GameCommand(std::string name,
        std::string description,
        std::function<bool(Player&, Map&, Game&)> action)
        : name(std::move(name)),
        description(std::move(description)),
        action(std::move(action)) {
    }

    bool execute(Player& player, Map& map, Game& game) override {
        if (action) {
            return action(player, map, game);
        }
        return true;
    }

    std::string getDescription() override {
        return description;
    }

    std::string getName() override {
        return name;
    }
};

#endif 