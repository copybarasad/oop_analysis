#include "SurrenderAction.h"

SurrenderAction::SurrenderAction(GameVisualizer<ConsoleRenderer>& view, Player& player): 
    view(view), player(player) {}

bool SurrenderAction::execute() {
    this->view.renderMessage("You've given up");
    this->player.setHP(0);
    return true;
}

std::string SurrenderAction::getName() const {
    return "Surrender";
}