#include "InfoAction.h"

InfoAction::InfoAction(GameVisualizer<ConsoleRenderer>& view, Hand& hand, Player& player):
    view(view), hand(hand), player(player) {}

bool InfoAction::execute() {
    this->view.renderPlayerInfo();
    return false;
}

std::string InfoAction::getName() const {
    return "Show info";
}