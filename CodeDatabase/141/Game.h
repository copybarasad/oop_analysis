#pragma once

#include "Field.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemyBuilding.h"
#include "Spell.h"
#include "GameSnapshot.h"
#include "Trap.h"
#include "EnemyTower.h"
#include "Ally.h"
#include "Enhancement.h"
#include "RenderSystem.h"
#include "CombatSystem.h"
#include "SpellSystem.h"
#include "MovementSystem.h"
#include "ISpellContext.h"
#include "EventBus.h"
#include "UpgradeSystem.h"
#include "GameConstants.h"
#include <vector>
#include <string>
#include <fstream>
#include <functional>

class Game : public ISpellContext {
public:
    Game(int fieldWidth, int fieldHeight, RenderSystem &renderer, EventBus *events = nullptr);

    bool isPlayerAlive() const;

    bool isGameRunning() const;

    void stop();

    void movePlayer(int dx, int dy);

    void playerAttack(int dx, int dy);

    void switchWeapon();

    void castSpellAt(int spellIndex, int dx, int dy);

    void saveTo(const std::string &path);

    void loadFrom(const std::string &path);

    void afterTurn();

    int getScoreTarget() const;

    void setUpgradeSelector(const std::function<int(const std::vector<UpgradeOption> &)> &selector);

    void towerAttackPlayer(int towerX, int towerY, int damage);

    bool isPlayerInRange(int centerX, int centerY, int radius) const;

    bool isPositionValidAndFree(int x, int y) const;

    void castTowerSpell(int sourceX, int sourceY, int targetX, int targetY, int range, int damage);

    void getPlayerPosition(int &x, int &y) const;

    void initializeLevel(int level);

    int getCurrentLevel() const;

    void setCurrentLevel(int level);

    int getFieldWidth() const;

    int getFieldHeight() const;

    void setFieldSize(int width, int height);

    int getEnemiesKilled() const;

    void setEnemiesKilled(int count);

    int getPlayerHealth() const;

    int getPlayerMeleeDamage() const;

    int getPlayerRangedDamage() const;

    int getPlayerScore() const;

    BattleType getPlayerBattleType() const;

    bool getPlayerSkipTurn() const;

    void setPlayerState(int x, int y, int health, int meleeDamage, int rangedDamage, int score, BattleType battleType,
                        bool skipTurn);

    const Hand &getPlayerHand() const;

    void clearPlayerHand();

    void addSpellToPlayer(Spell *spell);

    SpellType getSpellType(const Spell *spell) const;

    Spell *createSpellFromType(SpellType type);

    Spell *createSpellFromTypeAndData(std::ifstream &file, SpellType type);

    void setCellType(int x, int y, CellType type);

    void clearEnemies();

    void addEnemy(int x, int y, int health, int damage);

    void clearBuildings();

    void addBuilding(int x, int y, int spawnCooldown, int currentCooldown);

    void clearTowers();

    void addTower(int x, int y, int radius, int cooldown, int currentCooldown);

    void clearTraps();

    void addTrap(int x, int y, int damage, bool active);

    void clearAllies();

    void addAlly(int x, int y, int health, int damage);

    std::vector<Enhancement> getPendingEnhancements() const;

    void clearPendingEnhancements();

    void addPendingEnhancement(const Enhancement &enhancement);

    GameSnapshot makeSnapshot() const;

    void applySnapshot(const GameSnapshot &snapshot);

    Field &getField() override;

    const Field &getField() const;

    Player &getPlayer() override;

    const Player &getPlayer() const;

    std::vector<Enemy> &getEnemies() override;

    const std::vector<Enemy> &getEnemies() const;

    std::vector<EnemyBuilding> &getBuildings() override;

    const std::vector<EnemyBuilding> &getBuildings() const;

    std::vector<Trap> &getTraps() override;

    const std::vector<Trap> &getTraps() const;

    std::vector<Ally> &getAllies() override;

    const std::vector<Ally> &getAllies() const;

    const std::vector<EnemyTower> &getTowers() const override;

    void addMessage(const std::string &message) override;

    void addScore() override;

    void incrementKills(int count) override;

    void addEnhancement(const Enhancement &enhancement) override;

    void applyEnhancements(Spell &spell) override;

    void setEventBus(EventBus *bus);

private:
    Field field;
    Player player;
    std::vector<Enemy> enemies;
    std::vector<EnemyBuilding> buildings;
    std::vector<EnemyTower> towers;
    std::vector<Trap> traps;
    std::vector<Ally> allies;

    RenderSystem &renderSystem;
    CombatSystem combatSystem;
    SpellSystem spellSystem;
    MovementSystem movementSystem;
    EventBus *eventBus;

    bool gameRunning;
    int enemiesKilled;
    int currentLevel;
    std::function<int(const std::vector<UpgradeOption> &)> upgradeSelector;

    void render() const;

    void processEnemiesTurn();

    void processBuildingsTurn();

    void processTraps();

    void processTowersTurn();

    void processAlliesTurn();

    void handlePlayerMove(int dx, int dy);

    void handleLevelComplete();

    void handleUpgradeMenu();

    void spawnEnemyFromBuilding(EnemyBuilding &building);

    bool findSafeSpawnPosition(int &x, int &y, int maxAttempts = 50, int safeRadius = 5) const;

    bool isValidSpawnPosition(int x, int y, int safeRadius = 5) const;

    void placePlayerInSafeLocation();

    void placeEnemiesInSafeLocations();

    void placeEnemiesInSafeLocations(int level);

    void placeBuildingsInSafeLocations();

    void placeBuildingsInSafeLocations(int level);

    void placeTowersInSafeLocations();

    void placeTowersInSafeLocations(int level);

    Enemy createRandomEnemy(int x, int y);

    Enemy createRandomEnemy(int x, int y, int level);

    template<typename T, typename SpawnFunc>
    int spawnObjects(std::vector<T> &container, int desiredCount, int maxAttempts,
                     const std::string &objectName, SpawnFunc spawnFunc,
                     int safeRadius = GameConstants::SPAWN_SAFE_RADIUS);

    void showSpawnWarning(int placed, int expected, const std::string &objectType);

    void addRandomSpellToPlayer();

    bool findSafeSpawnPositionNearPlayer(int &x, int &y) const;

    void addScoreCallback();

    void incrementKillsCallback(int count);
};
