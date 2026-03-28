#include "shop_command.h"
#include "game_session.h"

void ShopCommand::Execute(GameSession& game) {
    game.ProcessOpenShop();
}
