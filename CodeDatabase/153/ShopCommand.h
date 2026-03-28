#ifndef SHOPCOMMAND_H
#define SHOPCOMMAND_H

#include "Command.h"

class ShopSystem;
class InputHandler;
class GameState;

class ShopCommand : public Command {
private:
    ShopSystem& shopSystem;
    InputHandler& inputHandler;

public:
    ShopCommand(ShopSystem& shop, InputHandler& handler);
    bool execute(GameState& gameState) override;
    std::string getName() const override;
};

#endif 