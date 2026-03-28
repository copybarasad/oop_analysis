#pragma once
#include <memory>
#include <string>
#include <SFML/System/Vector2.hpp>
#include "Field.h"
#include "Player.h"
#include "SpellFactory.h"

enum class GameState {
    Menu,
    Playing,
    LevelUpMenu,
    GameOver,
    Victory
};

class GameLogic {
private:
    std::unique_ptr<Field> field;
    Player* player = nullptr;
    SpellFactory spellFactory;

    GameState currentState;
    int currentLevel;
    bool player_turn;
    bool running;

    void initLevel();
    void nextLevel();

public:
    GameLogic();
    ~GameLogic();

    const Field* getField() const;
    Player* getPlayer() const;
    GameState getState() const;
    bool isRunning() const;
    bool isPlayerTurn() const;
    int getCurrentLevel() const;

    void startNewGame();
    void loadGame(const std::string& filename);
    void saveGame(const std::string& filename);
    void exitGame();

    void movePlayer(sf::Vector2i dir);
    void castSpell(int targetX, int targetY, int spellIndex);
    void buySpell();
    void toggleCombatMode();

    void upgradeHealth();
    void upgradeDamage();
    void upgradeSpell();

    void returnToMenu();
    void continueGame();

    void update();
};