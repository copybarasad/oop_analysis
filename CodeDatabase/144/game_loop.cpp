#include "game.h"
#include "game_controller.h"
#include "game_visualizer.h"
#include "console_command_reader.h"
#include "gamerenderer.h"
#include <iostream>

void Game::runGameLoop() {
    displayHelp();

	GameController<ConsoleCommandReader> controller{ConsoleCommandReader(controls)};
	GameVisualizer<GameRenderer> visualizer(*this);
    
    while (campaignActive) {
        while (isGameRunning()) {
			visualizer.renderTurn(turnCounter);
            GameCommand command = controller.nextCommand();
            controller.applyCommand(*this, command);
            update();
        }
        
        if (!campaignActive) {
            break;
        }
        
        if (!player.isAlive()) {
            onPlayerDefeat();
        } else if (!levelInProgress && !awaitingUpgradeSelection) {
            startLevel(currentLevelIndex);
        } else if (!gameRunning) {
            break;
        }
    }
}