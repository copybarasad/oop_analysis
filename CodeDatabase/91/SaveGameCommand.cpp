#include "SaveGameCommand.h"
#include "Game.h"

void SaveGameCommand::execute(Game& game) {
    game.executeSaveLogic();
}