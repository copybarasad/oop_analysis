#include "MoveAction.h"

MoveAction::MoveAction(GameVisualizer<ConsoleRenderer>& view, int dx, int dy, const std::string& dir, Field& field, Player& player) : 
    view(view), dx(dx), dy(dy), direction(dir), field(field), player(player) {}
    
bool MoveAction::execute() {
    if (MovementSystem().step(this->player, dx, dy, this->field)) {
        this->view.renderMessage("You have moved to the " + direction);
        return true;
    }
    this->view.renderMessage("Couldn't move");
    return false;
}

std::string MoveAction::getName() const {
    return "Move " + direction;
}