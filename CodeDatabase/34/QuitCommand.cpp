#include "QuitCommand.h"
#include "../Game.h"

namespace Game {

void QuitCommand::execute(Game& game) {
    game.setResult(LevelResult::Quit);
}

}
