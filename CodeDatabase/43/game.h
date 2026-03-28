#ifndef GAME_H
#define GAME_H

#include "game_field.h"
#include "level_manager.h"
#include "player.h"
#include "enemy.h"
#include "spell.h"
#include "direct_damage_spell.h"
#include "area_damage_spell.h"
#include "game_state.h"
#include <memory>
#include <vector>

class Game {
private:
    std::unique_ptr<GameField> field;
    std::shared_ptr<Player> player;
    std::vector<std::shared_ptr<Enemy>> enemies;
    bool gameRunning;
    int playerX, playerY;
    LevelManager levelManager;
    
    void giveNewSpellOnFirstKill();
    void spawnInitialEntities();
    void handlePlayerMovement(int dx, int dy);
    void handleRangedAttack();
    void handleSpellCast(int spellIndex, int targetX, int targetY);
    void handleSpellCastOnNearestEnemy(int spellIndex);
    void handleEnemyTurns();
    void tryAlternativeDirections(int enemyX, int enemyY);
    void removeDeadEnemies();
    void findPlayerPosition();
    GameState createGameState() const;
    void applyGameState(const GameState& state);
    void spawnEnemiesForCurrentLevel();

public:
    Game();
    ~Game() = default;

    void initializeGame();
    void updateGame();
    void checkGameConditions();
    void goToNextLevel();
    
    // === Старые методы сохранения/загрузки ===
    void saveGame(const std::string& filename);
    void loadGame(const std::string& filename);
    void restartGame();
    bool isGameRunning() const;
    
    // === Новые методы для команд 4-й ЛР ===
    void movePlayer(int dx, int dy);
    void switchCombatMode();
    void rangedAttack();
    void castSpell(int index);
    void save();
    void load();
    void restart();
    void quit();
    void displayMessage(const std::string& message);
    // В game.h в public секции:
    void quitToMenu();  // выход в меню (без завершения программы)
    void quitGame();    // полный выход (удали этот метод или сделай алиас)
    
    // === Геттеры для визуализатора ===
    int getPlayerX() const { return playerX; }
    int getPlayerY() const { return playerY; }
    const Player* getPlayer() const { return player.get(); }
    const GameField* getField() const { return field.get(); }
    const std::vector<std::shared_ptr<Enemy>>& getEnemies() const { return enemies; }
    int getCurrentLevel() const { return levelManager.getCurrentLevel(); }
    int getEnemiesDefeated() const;
    int getTurnNumber() const;
    
    // === Методы для работы с заклинаниями ===
    void addSpellToPlayer(std::shared_ptr<Spell> spell);
    GameField* getField() { return field.get(); }
    
    
    // === Старый метод ввода (можно оставить пустым или удалить) ===
    // void processInput(char input); // Удаляем или делаем пустым
};

#endif