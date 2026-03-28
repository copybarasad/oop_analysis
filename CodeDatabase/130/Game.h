#ifndef GAME_H
#define GAME_H

#include "GameField.h"
#include "Player.h"
#include "EnemyManager.h"
#include "BuildingManager.h"
#include "TrapManager.h"
#include "AllyManager.h"
#include "TowerManager.h"
#include "Hand.h"
#include "Level.h"
#include "PlayerUpgrade.h"
#include "GameSaveManager.h"
#include "GameState.h"
#include "InputConfig.h"
#include "EventBus.h"
#include "Command.h"
#include <memory>
#include <vector>

struct GameRenderData;

class Game {
public:
    Game();
    ~Game() = default;

    // Инициализация игры
    void initializeGame();
    
    // Проверка состояния
    bool isGameRunning() const;
    int getCurrentTurn() const;
    
    // Управление игрой (для внешнего контроллера)
    bool handleCommand(const Command& command);
    void processEnemyTurns();
    void processTurnEnd();
    bool canProcessInput() const;
    void skipSlowedTurn();
    
    // Save/Load
    bool loadGame();
    void saveGame();
    void autoSaveGame();

    // Данные для рендеринга (для внешнего визуализатора)
    GameRenderData getRenderData() const;

    // Event bus
    void setEventBus(const std::shared_ptr<EventBus>& bus);
    
    // Input config
    InputConfig& getInputConfig();
    const InputConfig& getInputConfig() const;

    // Устаревшие методы для обратной совместимости
    void startGame();
    void runGameLoop();
    void displayGameState(); // deprecated, use Renderer

private:
    // Initialization
    void loadLevel(int levelNum);
    void setupEnemies(int count, int hp, int damage);
    void setupBuildings(int count, int cooldown);
    void setupTowers(int count);
    void initializeHand();

    // Actions
    bool movePlayerTo(int dx, int dy);
    bool attackInDirection(int dx, int dy);
    void nearbyEnemiesAttackPlayer();
    bool castSpellCommand();
    bool saveGameCommand();
    void showQuitMenu();
    void tryGivePlayerSpell();

    // Level progression
    void transitionToNextLevel();
    void removeHalfSpells();
    void showUpgradeMenu();
    void applyUpgrade(UpgradeType upgrade);
    void checkGameEnd();

    // Save/Load (internal)
    GameState createGameState() const;
    void restoreFromGameState(const GameState& state);
    bool loadAutoSave();
    
    // Death handling
    enum class DeathChoice { NEW_GAME, RESTORE_LEVEL, LOAD_SAVE, EXIT };
    DeathChoice showDeathMenu();
    void handlePlayerDeath();

private:
    std::unique_ptr<GameField> field_;
    std::unique_ptr<Player> player_;
    std::unique_ptr<EnemyManager> enemyManager_;
    std::unique_ptr<BuildingManager> buildingManager_;
    std::unique_ptr<TrapManager> trapManager_;
    std::unique_ptr<AllyManager> allyManager_;
    std::unique_ptr<TowerManager> towerManager_;
    std::unique_ptr<Hand> hand_;
    std::unique_ptr<GameSaveManager> saveManager_;
    InputConfig inputConfig_;
    std::shared_ptr<EventBus> eventBus_;
    
    bool gameRunning_;
    int currentTurn_;
    int enemiesKilled_;
    int currentLevel_;
    bool hasUsedRevive_;
    std::vector<UpgradeType> appliedUpgrades_;
};

#endif
