#pragma once

#include "game_field.h"
#include "player.h"
#include "enemy.h"
#include "spell_shop.h"
#include "position.h"
#include "move_direction.h"
#include "game_exceptions.h"
#include "binary_file.h"

#include <vector>
#include <string>

class SpellCard;

class GameController {
public:
    static constexpr uint32_t MAGIC = 0x524F474C; 
    static constexpr uint8_t VERSION = 1;

    GameController(int fieldWidth, int fieldHeight, int initialEnemyCount = 3);

    void processPlayerMove(MoveDirection direction);
    bool castSpell(int spellIndex, const Position& target);
    bool buySpell(int shopIndex);

    void updateGame();

    bool isGameRunning() const { return gameActive; }
    void quitGame() { gameActive = false; }

    // Геттеры для рендера и ввода
    const Player& getPlayer() const { return player; }
    Player& getPlayer() { return player; }
    const std::vector<Enemy>& getEnemies() const { return enemies; }
    const GameField& getGameField() const { return gameField; }
    const SpellShop& getSpellShop() const { return spellShop; }
    int getCurrentLevel() const { return currentLevel; }

    bool saveGame(const std::string& filename = "autosave.bin") const;
    bool loadGame(const std::string& filename = "autosave.bin");

    void resetGame();
    void startNewGame();

private:
    void initializeGame(int enemyCount);
    void placePlayerAtStart();
    void placeEnemies(int enemyCount);
    bool tryMovePlayer(const Position& newPos);
    Position calculateNewPosition(MoveDirection dir) const;

    void moveEnemies();
    void applyDamageFromNearbyEnemies();
    void checkGameConditions();
    void checkSpellRewards();

    void saveImpl(std::ostream& out) const;
    void loadImpl(std::istream& in);

    GameField gameField;
    Player player;
    std::vector<Enemy> enemies;
    SpellShop spellShop;

    bool gameActive = true;
    int enemiesKilled = 0;
    int currentLevel = 1;
};