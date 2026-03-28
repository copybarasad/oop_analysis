#ifndef GAME_H
#define GAME_H

#include <vector>
#include <memory>
#include "Player.h"
#include "Enemy.h"
#include "Ally.h"
#include "EnemyTower.h"
#include "Trap.h"
#include "GameField.h"
#include "GameTypes.h"
#include "LevelManager.h"
#include "PlayerUpgrade.h"

class SpellHand;
class LevelBuilder;
class TurnProcessor;

/**
 * @file Game.h
 * @brief Основной класс игры
 */
class Game {
    friend class LevelBuilder;
    friend class TurnProcessor;

private:
    std::unique_ptr<GameField> field;
    std::unique_ptr<Player> player;
    std::vector<std::unique_ptr<Enemy>> enemies;
    std::vector<std::unique_ptr<Ally>> allies;
    std::vector<std::unique_ptr<EnemyTower>> towers;
    std::vector<std::unique_ptr<Trap>> traps;
    std::unique_ptr<SpellHand> spellHand;

    bool gameRunning;

    LevelManager levelManager;
    PlayerUpgradeSystem upgradeSystem;

    void removeDeadEnemies();
    void removeDeadAllies();
    void removeActivatedTraps();
    void processAfterPlayerAction();
    void processAllyTurns();
    void processTowerTurns();
    void checkTrapActivations();
    void checkCollisions();
    void initializeLevel(const LevelConfig& config);

public:
    Game();
    ~Game();

    void initializeGame();
    
    /**
     * @brief Начать новую игру (сброс после Game Over)
     */
    void startNewGame();
    
    void processPlayerTurn(Direction direction);
    void processEnemyTurns();
    void switchPlayerAttackMode();

    bool isGameRunning() const noexcept { return gameRunning; }

    void spawnEnemy(int x, int y);
    void spawnEnemy(int x, int y, int healthBonus, int damageBonus);
    void spawnTower(int x, int y, int health, int range);

    bool isValidMove(int x, int y) const;
    void handleCombat(Enemy* enemy);

    void addAlly(std::unique_ptr<Ally> ally);
    void addTower(std::unique_ptr<EnemyTower> tower);
    void addTrap(std::unique_ptr<Trap> trap);

    /**
     * @brief Использовать заклинание без координат (Boost, Direct Damage)
     */
    bool usePlayerSpell(int spellIndex);
    
    /**
     * @brief Использовать заклинание с координатами (Area Damage, Trap)
     */
    bool usePlayerSpell(int spellIndex, int targetX, int targetY);
    
    void addRandomSpellToHand();

    void saveGame(const std::string& saveName);
    void loadGame(const std::string& saveName);

    void loadPlayerState(int health, int maxHealth, int damage, int x, int y, int score, int kills, AttackMode mode);
    void loadSpellHandState(int spellCount, int maxSize, int boostStacks);

    void nextLevel();
    void gameOver();
    
    SpellHand* getSpellHand() { return spellHand.get(); }
    const SpellHand* getSpellHand() const { return spellHand.get(); }
    
    SpellHand* getPlayerSpellHand() { return spellHand.get(); }
    const SpellHand* getPlayerSpellHand() const { return spellHand.get(); }
    
    /**
     * @brief Получить систему улучшений
     * @return Указатель на систему улучшений
     */
    PlayerUpgradeSystem* getUpgradeSystem() { return &upgradeSystem; }
    
    /**
     * @brief Применить улучшение игроку
     * @param upgrade Улучшение для применения
     */
    void applyUpgrade(const Upgrade* upgrade);

    void recreateField(int width, int height) {
        field = std::make_unique<GameField>(width, height);
    }

    Player* getPlayer() const { return player.get(); }

    GameField* getField() { return field.get(); }
    const GameField* getField() const { return field.get(); }

    std::vector<std::unique_ptr<Enemy>>& getEnemies() { return enemies; }
    std::vector<std::unique_ptr<Ally>>& getAllies() { return allies; }
    std::vector<std::unique_ptr<EnemyTower>>& getTowers() { return towers; }
    std::vector<std::unique_ptr<Trap>>& getTraps() { return traps; }

    const std::vector<std::unique_ptr<Enemy>>& getEnemies() const { return enemies; }
    const std::vector<std::unique_ptr<Ally>>& getAllies() const { return allies; }
    const std::vector<std::unique_ptr<EnemyTower>>& getTowers() const { return towers; }
    const std::vector<std::unique_ptr<Trap>>& getTraps() const { return traps; }

    int getCurrentLevel() const { return levelManager.getCurrentLevel(); }
    void setCurrentLevel(int level) { levelManager.setCurrentLevel(level); }
    
    /**
     * @brief Проверить, запущена ли игра
     * @return true если игра продолжается, false если закончена
     */
    bool isRunning() const noexcept {
        return gameRunning;
    }
};

#endif // GAME_H


