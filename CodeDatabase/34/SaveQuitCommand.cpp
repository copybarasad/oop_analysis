#include "SaveQuitCommand.h"
#include "../Game.h"

namespace Game {

void SaveQuitCommand::execute(Game& game) {
    game.setResult(LevelResult::SaveAndQuit);
}

}
