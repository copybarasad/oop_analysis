#include "togglemode.hpp"
#include "gamemodel.hpp"
#include "player.hpp"

bool ToggleModeCommand::Execute(Game& game) {
    game.getPlayer().ToggleMode();
    return true;
}