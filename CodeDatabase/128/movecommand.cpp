#include "movecommand.hpp"
#include "gamemodel.hpp" 

MoveCommand::MoveCommand(int dx, int dy) : dx_(dx), dy_(dy) {}

bool MoveCommand::Execute(Game& game) {
    return game.getMove().MovePlayer(dx_, dy_);
}