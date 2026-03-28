#include "MoveCommand.h"
#include "../Game.h"

namespace Game {

MoveCommand::MoveCommand(int dr, int dc) : dr(dr), dc(dc) {}

void MoveCommand::execute(Game& game) {
    game.movePlayer(dr, dc);
}

}
