#ifndef GAME_H
#define GAME_H

#include "Player.h"
#include "Enemy.h"
#include "GameField.h"
#include "Spell.h"
#include "SingleTargetSpell.h"
#include "AreaSpell.h"
#include "Command.h"
#include <vector>

class Game {
private:
    GameField gameField;
    Player mainPlayer;
    std::vector<Enemy> enemyList;
    bool isGameActive;
    int currentTurn;

    void setupGame();
    void applyMeleeAttack(Player& attacker, Enemy& target);
    void applyRangedAttack(int targetX, int targetY);
    bool validateRangedAttack(int targetX, int targetY);
    void applyEnemyAttack(Enemy& attacker, Player& target);
    void removeDeadEnemies();
    
    bool processSpellCast(int spellIndex, int targetX, int targetY);
    
    void giveRandomSpellToPlayer();
    void initializeSpells();
    int lastSpellScore;
    
    bool castSingleTargetSpell(SingleTargetSpell* spell, int targetX, int targetY);
    bool castAreaSpell(AreaSpell* spell, int targetX, int targetY);
    bool isValidSingleTarget(int targetX, int targetY, int range) const;
    bool isValidAreaTarget(int targetX, int targetY, int range) const;

    void restartGame();
    void handleSaveGame();

public:
    Game();
    
    // Основной метод обработки команд из InputHandler
    bool executeCommand(const Command& command);

    // Getters
    Player& getPlayer();
    const Player& getPlayer() const;
    const std::vector<Enemy>& getEnemies() const;
    std::vector<Enemy>& getEnemies();
    const GameField& getField() const { return gameField; }
    
    int getCurrentTurn() const { return currentTurn; }
    bool getIsGameActive() const { return isGameActive; }
    void setIsGameActive(bool active) { isGameActive = active; }
    void setCurrentTurn(int turn) { currentTurn = turn; }
    
    // Public logic methods called by Game Manager
    void handleEnemyTurns();
    void checkGameCompletion();
    void checkForNewSpell();
    void refreshGameState() { removeDeadEnemies(); }
    void showGameOverStats() const;
    
    bool hasPlayerWon() const;
    bool hasPlayerLost() const;
    void createEnemies();
    
    bool castSpell(Spell* spell, int targetX, int targetY);
};

#endif