#ifndef RPG_DIRECTIONCOMMAND_H
#define RPG_DIRECTIONCOMMAND_H

#include "Command.h"
#include "../Game.h"
#include "../Player.h"

enum Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

class DirectionCommand : public Command {
    std::pair<int,int> direction;
    std::shared_ptr<Game>& game;
public:
    DirectionCommand(Direction dir, std::shared_ptr<Game>& game) : direction(0, 0), game(game) {
        switch (dir) {
            case LEFT:
                direction.first = -1;
                break;
            case RIGHT:
                direction.first = 1;
                break;
            case UP:
                direction.second = -1;
                break;
            case DOWN:
                direction.second = 1;
                break;
        }
    }
    
    std::pair<bool, std::string> execute() override {
        auto& player = game->getPlayer();
        auto mode = player.getMode();
        if (mode ==  PlayerModes::RANGE) {
            if (game->checkAimBorders(direction, &player))
                return {false, ""};
            return {false, "Выход за границы прицеливания\n"};
        } if (mode ==  PlayerModes::MELEE) {
            if (game->moveEntity(direction, &player))
                return {true, ""};
            return {false, "Неудачное направление движения\n"};
        } if (mode ==  PlayerModes::SPELL) {
            auto [tx, ty] = player.getTarget();
            auto [dx, dy] = direction;
            auto [x, y] = player.getPosition();
            std::pair<int, int> difference = {tx + dx - x, ty + dy - y};
            if (!player.getSpellsHand().getSelectedSpell()->allCellsInRange(difference))
                return {false, "Выход за границы применения заклинания\n"};
            if (game->checkAimSpellBorders(direction, &player))
                return {false, ""};
            return {false, "Выход за границы применения заклинания\n"};
        } if (mode ==  PlayerModes::UPGRADE) {
            if (direction.second == -1) {
                player.upgradeHealth();
                return {false, ""};
            } if (direction.second == 1) {
                player.upgradeDamage();
                return {false, ""};
            }
        }
        return {false, ""};
    }
};

#endif