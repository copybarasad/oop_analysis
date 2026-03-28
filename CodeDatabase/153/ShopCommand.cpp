#include "ShopCommand.h"
#include "GameState.h"
#include "ShopSystem.h"
#include "Player.h"

ShopCommand::ShopCommand(ShopSystem& shop, InputHandler& handler)
    : shopSystem(shop), inputHandler(handler) {
}

bool ShopCommand::execute(GameState& gameState) {
    return shopSystem.interactWithPlayer(gameState.getPlayer(), inputHandler);
}

std::string ShopCommand::getName() const {
    return "ShopCommand";
}