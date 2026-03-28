#ifndef GAME_H
#define GAME_H

#include "GameWorld.h"
#include "Player.h"
#include "GameRenderer.h"
#include "SpellHand.h"
#include "PlayerAction.h"
#include "EnhancementBuffer.h"
#include "LevelManager.h"
#include "GameSnapshot.h"
#include "Direction.h"
#include "Position.h"
#include "InputReader.h"
#include "Command.h"
#include <memory>

class SaveGameManager;

class Game {
private:
    int fieldSize;
    std::unique_ptr<GameWorld> world;
    std::unique_ptr<Player> player;
    std::unique_ptr<SpellHand> spellHand;
    EnhancementBuffer enhancementBuffer;
    GameRenderer renderer;
    LevelManager levelManager;
    PlayerProgress playerProgress;
    
    InputReader<ConsoleInputHandler> controller;
    
    int turnNumber;
    bool isRunning;
    bool shouldRestart;
    bool levelCompleted;
    
    int lastTurnDamageDealt;
    int lastTurnEnemiesHit;
    int lastTurnEnemiesKilled;
    
    bool processPlayerTurn();
    void processEnemyTurn();
    bool handlePlayerAction(const PlayerAction& action);
    bool processCommand(std::unique_ptr<Command> cmd);
    void checkGameStatus();
    void checkLevelCompletion();
    void handleGameOver();
    void handleVictory();
    
    void handleLevelTransition();
    void handlePlayerUpgrade(int& meleeDamageUpgrade, int& rangedDamageUpgrade, int& healthBonus);
    void trimSpellHand();

public:
    Game(int fieldSize, GameMode mode = GameMode::CUSTOM);
    
    char getCellSymbol(int x, int y) const;
    
    void initialize();
    void initializeLevel(int level);
    void run();
    bool getShouldRestart() const;
    
    GameSnapshot exportState() const;
    void importState(const GameSnapshot& snapshot);
    
    const Player& getPlayer() const;
    const GameWorld& getWorld() const;
    const SpellHand& getSpellHand() const;
    const EnhancementBuffer& getEnhancementBuffer() const;
    const PlayerProgress& getPlayerProgress() const;
    int getTurnNumber() const;
    int getCurrentLevel() const;
    int getFieldSize() const { return fieldSize; }
    
    bool executeMove(Direction dir);
    bool executeAttack(Direction dir);
    bool executeSwitchMode();
    bool executeSkipTurn();
    bool executeCastSpell(int spellIndex, Position target);
    bool executeBuySpell();
    bool executeSaveGame();
    bool executeRestart();
    bool executeQuit();
};

#endif
