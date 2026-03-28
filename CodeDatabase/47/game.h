#ifndef GAME_H
#define GAME_H

#include "game_state.h"
#include "save_manager.h"
#include "level_manager.h"
#include "upgrade_system.h"
#include "game_exceptions.h"
#include "spell.h"
#include "player.h"
#include "enemy.h"
#include "enemy_building.h"
#include "enemy_tower.h"
#include "trap.h"
#include "game_field.h"
#include "cell_type.h"
#include "combat_mode.h"
#include "command.h"

#include <memory>

class Game {
private:
    std::unique_ptr<GameState> gameState;

    bool runningFlag;
    int extraEnemyTurns;

    int getValidInput(int minChoice, int maxChoice);
    void initializeLevel(int level);
    bool tryMovePlayer(int dx, int dy);
    void processPlayerAttack();
    bool processSpellCast(char spellKey);
    bool processSpellCastAtIndex(int index, int x, int y);
    void processEnemyTurn();
    void spawnEnemyFromBuilding();
    void checkTraps();
    void checkGameState();
    void drawField();
    bool isPositionOccupied(int x, int y) const;
    void tryAddNewSpell();

public:
    Game();
    ~Game() = default;

    void startNewGame();
    bool loadGame();
    bool loadGameFromMenu();
    void saveGame();
    void resetGame();
    bool isRunningFlag() const { return runningFlag; }

    bool applyCommand(const Command& cmd);

    bool isGameOver() const;
    bool isLevelCompleted() const;
    int getCurrentLevel() const;
    Player& getPlayer();
    GameState& getGameState();
    int getTotalScore() const;
    
    void enemyTurn();
    void performPostEnemyChecks();
    int getExtraEnemyTurns() const { return extraEnemyTurns; }
    void consumeExtraEnemyTurn() { if (extraEnemyTurns > 0) extraEnemyTurns--; }
    void drawFieldPublic() { drawField(); }
    int getPlayerPositionX() const;
    int getPlayerPositionY() const;

    bool damageEnemyAtPosition(int x, int y, int dmg);
    bool damageBuildingAtPosition(int x, int y, int dmg);
    bool placeTrap(int x, int y, int dmg);
    bool hasEnemyOrBuildingAt(int x, int y) const;
    bool isPositionValid(int x, int y) const;
    bool isPositionPassable(int x, int y) const;
    void damagePlayer(int dmg);
    void handleLevelCompletion();
};

#endif
