#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include <vector>
#include <iostream>
#include <memory>
#include "GameField.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemyBuilding.h"
#include "AttackResult.h"
#include "Hand.h"
#include "Trap.h"
#include "Ally.h"
#include "EnemyTower.h"
#include "MovementSystem.h"
#include "CombatSystem.h"
#include "EnemyAISystem.h"
#include "SpellSystem.h"
#include "BuildingManager.h"
#include "AllyManager.h"
#include "TrapManager.h"
#include "RenderSystem.h"
#include "PlayerActionSystem.h"
#include "CharacterFactory.h"
#include "SpellFactory.h"
#include "BuildingFactory.h"
#include "LevelManager.h"
#include "GameNotifier.h"
#include "GameObserver.h"
#include "Logger.h"

class GameController : public IGameNotifier {
private:
    GameField field;
    Player player;
    std::vector<Enemy> enemies;
    std::vector<EnemyBuilding> buildings;
    std::vector<Trap> traps;
    std::vector<Ally> allies;
    std::vector<EnemyTower> towers;
    Hand hand;
    bool gameRunning;
    
    MovementSystem movementSystem;
    CombatSystem combatSystem;
    EnemyAISystem enemyAISystem;
    BuildingManager buildingManager;
    AllyManager allyManager;
    RenderSystem renderSystem;
    PlayerActionSystem playerActionSystem;
    
    std::vector<std::shared_ptr<IGameObserver>> observers;
    
public:
    GameController(int fieldWidth, int fieldHeight);
    ~GameController() = default;
    
    bool isGameRunning() const;
    void processPlayerMove(const Position &direction);
    void processPlayerAttack(const Position &direction);
    void processPlayerAreaAttack();
    void processPlayerSwitchCombat();
    void renderGame() const;
    bool useSpell(int handIndex, const Position &target);
    bool buyRandomSpell(int cost);
    bool upgradeSpell(int spellIndex);
    void prepareForNewLevel();
    
    void addObserver(std::shared_ptr<IGameObserver> observer);
    void removeObserver(std::shared_ptr<IGameObserver> observer);
    
    void notifyPlayerMoved(const Position& from, const Position& to) override;
    void notifyPlayerAttacked(const std::string& target, int damage) override;
    void notifyPlayerDamaged(int damage, const std::string& source) override;
    void notifyEnemyKilled(const Position& position, int coinsReward) override;
    void notifyTowerDestroyed(const Position& position) override;
    void notifySpellUsed(const std::string& spellName, const Position& target) override;
    void notifyCommandExecuted(const std::string& command) override;
    
    void notifyLevelUp(int newLevel);
    void notifyGameSaved();
    void notifyGameLoaded();
    void notifyGameStarted();
    void notifyGameEnded();
    
    Hand &getHand() { return hand; }
    const Hand &getHand() const { return hand; }
    const Player &getPlayer() const { return player; }
    Player &getPlayerMutable() { return player; }
    const GameField &getField() const { return field; }
    GameField &getFieldMutable() { return field; }
    std::vector<Enemy>& getEnemies() { return enemies; }
    std::vector<EnemyBuilding>& getBuildings() { return buildings; }
    std::vector<EnemyTower>& getTowers() { return towers; }
    std::vector<Trap>& getTraps() { return traps; }
    std::vector<Ally>& getAllies() { return allies; }
    const std::vector<Enemy>& getEnemies() const { return enemies; }
    const std::vector<EnemyBuilding>& getBuildings() const { return buildings; }
    const std::vector<EnemyTower>& getTowers() const { return towers; }
    const std::vector<Trap>& getTraps() const { return traps; }
    const std::vector<Ally>& getAllies() const { return allies; }
    
    Enemy *getEnemyAtPosition(const Position &position);
    EnemyBuilding* getBuildingAtPosition(const Position &pos);
    EnemyTower* getTowerAtPosition(const Position &pos);
    
    void addTrap(const Trap &t);
    void removeTrapAt(const Position &pos);
    void addAlly(const Ally &a);
    void addTower(const EnemyTower &t);
    void removeDeadEnemies();
    void addScore(int points) { player.addScore(points); }
    void addCoins(int coins) { player.addCoins(coins); }
    
private:
    void cleanupDeadEnemies();
};

#endif