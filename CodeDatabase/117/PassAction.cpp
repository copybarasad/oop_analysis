#include "PassAction.h"

PassAction::PassAction(GameVisualizer<ConsoleRenderer>& view) : view(view) {}

bool PassAction::execute() {
    this->view.renderMessage("You missed a move");
    return true;
}

std::string PassAction::getName() const {
    return "Pass turn";
}