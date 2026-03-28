#include "game.h"
#include <algorithm>
#include "game_ally_action_context.h"
#include "game_trap_action_context.h"
#include "game_enemy_movement_context.h"

void Game::update() {
    if (playerActionTaken) {
		GameEnemyMovementContext movementContext(*this);
        enemyManager.moveEnemies(field, player, movementContext, &eventBus);
        buildingManager.updateBuildings(field, enemyManager, currentEnemyHealth, currentEnemyDamage);
        buildingManager.updateTowers(*this);
        updateAllies();
        checkTraps();
        if (turnCounter % 5 == 0 && !player.getHand().isFull()) {
            giveRandomSpell();
        }
        
        turnCounter++;
        std::cout << "Turn " << turnCounter << " completed.\n";
    }
    
    if (!player.isAlive()) {
        gameRunning = false;
        std::cout << "Game Over! Final Score: " << player.getScore() << "\n";
		logEvent(GameEventType::System, "Player died. Final score: " + std::to_string(player.getScore()));
    }

    if (levelInProgress && isLevelCleared()) {
        handleLevelCompletion();
    }
}

void Game::updateAllies() {
	GameAllyActionContext allyContext(*this);
    for (auto& ally : allies) {
        ally.update(allyContext);
    }
    allies.erase(std::remove_if(allies.begin(), allies.end(), 
                               [](const Ally& ally) { return !ally.isAlive(); }), 
                 allies.end());
}

void Game::checkTraps() {
	GameTrapActionContext trapContext(*this);
	for (auto& trap : traps) {
		trap.checkAndTrigger(trapContext);
	}
	traps.erase(std::remove_if(traps.begin(), traps.end(), 
	           [](const Trap& trap) { return !trap.getIsActive(); }), 
	           traps.end());
}