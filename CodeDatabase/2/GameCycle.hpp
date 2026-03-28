#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "Board.hpp"
#include "Player.hpp"
#include "Enemy.hpp"
#include "EnemyTower.hpp"
#include "EnemyAttackTower.hpp"
#include "Ally.hpp"
#include "PlayersHand.hpp"
#include "Spell.hpp"
#include "LevelUpSystem.hpp"
#include "SaveManager.hpp"
#include "SaveLoadMenu.hpp"

class GameCycle {
private:
    std::unique_ptr<Board> board;
    std::unique_ptr<Player> player;
    std::unique_ptr<EnemyTower> spawnTower;
    std::unique_ptr<EnemyAttackTower> attackTower;
    std::unique_ptr<PlayersHand> playerHand;

    std::vector<Enemy> enemies;
    std::vector<Ally> allies;

    sf::Texture texture_player, texture_enemy, texture_ally;
    sf::Texture texture_empty_cell, texture_obstacle_cell, texture_slow_cell, texture_trap_cell;
    sf::Texture texture_tower, texture_attack_tower;
    sf::Font font;

    std::unique_ptr<sf::Sprite> sprite_player, sprite_enemy, sprite_ally;
    std::unique_ptr<sf::Sprite> sprite_empty_cell, sprite_obstacle_cell, sprite_slow_cell, sprite_trap_cell;
    std::unique_ptr<sf::Sprite> sprite_tower, sprite_attack_tower;

    sf::Text hp_text, mana_text, mode_text, turn_text, spell_text;
    sf::Text game_over_text, victory_text, casting_text;

    int currentLevel;
    bool playerTurn;
    bool gameOver;
    bool victory;
    bool castingSpell;
    int selectedSpellIndex;

    LevelUpSystem levelUpSystem;

    SaveManager saveManager;
    SaveLoadMenu saveLoadMenu;

    void initializeLevel();
    void processPlayerInput(char input);
    void processEntityTurn();
    void cleanupDeadEntities();
    void checkWinCondition();
    void checkLossCondition();
    void setupNextLevel();
    void resetTurnStates();
    void updateBoardPositions();
    void restartGameForNextLevel(int length, int width);

    void loadTextures();
    void loadFonts();
    void setupUI();
    void updateUI();;
    void drawUI(sf::RenderWindow& window);
    void processInput(sf::RenderWindow& window);
    void render(sf::RenderWindow& window);
    void showGameOverScreen(sf::RenderWindow& window);
    void showLevelUpScreen(sf::RenderWindow& window);
    void handleSpellTargetSelection(const sf::RenderWindow& window);
    void showSaveLoadMenu(bool isSaveMenu);
    void quickSave(int slot = 1);
    void quickLoad(int slot = 1);
    void autoSave();

public:
    GameCycle();
    ~GameCycle() = default;

    void run();
    void saveGame(const std::string& filename) const;
    void loadGame(const std::string& filename);
    void startNewGame(int length, int width);
    void processSpellCast(int spellIndex, int targetX = -1, int targetY = -1);
    void handleMouseClick(int mouseX, int mouseY, float offsetX, float offsetY, float cellSize);
    void logVictoryStats();

    // Геттеры для UI
    bool isPlayerTurn() const { return playerTurn; }
    bool isGameOver() const { return gameOver; }
    bool isVictory() const { return victory; }
    int getCurrentLevel() const { return currentLevel; }
    const Player* getPlayer() const { return player.get(); }
    const PlayersHand* getPlayerHand() const { return playerHand.get(); }
    const Board* getBoard() const { return board.get(); }
    const std::vector<Enemy>& getEnemies() const { return enemies; }
    const std::vector<Ally>& getAllies() const { return allies; }
    bool isCastingSpell() const { return castingSpell; }
    int getSelectedSpellIndex() const { return selectedSpellIndex; }
    const EnemyTower* getSpawnTower() const { return spawnTower.get(); }
    const EnemyAttackTower* getAttackTower() const { return attackTower.get(); }
    const SaveManager& getSaveManager() const { return saveManager; }

    void setCastingSpell(bool casting, int spellIndex = -1) {
        castingSpell = casting;
        selectedSpellIndex = spellIndex;
    }
};