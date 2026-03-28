#ifndef RPG_SHOPCOMMAND_H
#define RPG_SHOPCOMMAND_H

#include "Command.h"
class Player;

class ShopCommand : public Command {
    std::shared_ptr<Game>& game;
public:
    explicit ShopCommand(std::shared_ptr<Game>& game) : game(game) {}

    std::pair<bool, std::string> execute() override {
        game->getPlayer().setMode(PlayerModes::SHOP);
        return {false, "Добро пожаловать в магазин!\n"};
    }
};

#endif