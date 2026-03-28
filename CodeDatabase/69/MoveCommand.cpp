#include "MoveCommand.h"
#include "../Game.h"
#include "../Player.h"

#include <string>

MoveCommand::MoveCommand(int dx, int dy) : dx_(dx), dy_(dy) {}

CommandResult MoveCommand::execute(Game& game) {
    if (auto* player = game.GetPlayer()) {
        if (player->Move({dx_, dy_}, game)) {
            return {true, "Moved to (" + std::to_string(player->GetPosition().x) + "," + std::to_string(player->GetPosition().y) + ")"};
        } else {
            return {false, "Move failed."};
        }
    }
    return {false, "No player found."};
}
