#pragma once
#ifndef ICOMMAND_H
#define ICOMMAND_H

#include <string>

class Player;
class Map;
class Game;

class ICommand {
public:
    virtual ~ICommand() = default;

    virtual bool execute(Player& player, Map& map, Game& game) = 0;

    virtual std::string getDescription() = 0;

    virtual std::string getName() = 0;
};

#endif 