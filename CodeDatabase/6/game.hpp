#ifndef GAME
#define GAME

#include <memory>
#include <string>
#include <vector>

#include "gameManager.hpp"
#include "saveManager.hpp"
#include "gameState.hpp"
#include "gameRender.hpp"

#include "../gui_headers/keyBindingsInfo.hpp"

class Game {
    private:
        std::unique_ptr <GameManager> gameManager;
        SaveManager saveManager;
        GameState currentState;
        int currentLevel;
        bool gameRunning;

        GameRenderer<GameGUI> renderer;
        KeyBindingsInfo currentKeyBindings;

        void executeGameTurn();

        void startNewGame();
        void startNewLevel();
        
        void loadGame();

    public:
        Game();

        void update();
        void render();

        void movePlayer(direction dir);
        void toggleFight();
        void showAttackRadius();
        void enterBuySpellMode();
        void castSpell(int spellIndex);
        void cancelAction();
        void startNewGameCommand();
        void saveGameCommand();
        void continueGame();
        void exitGame();
        void continueToNextLevel();
        void returnToMainMenu();
        void applyUpgrade(int upgradeIndex);
        void buySpell(int spellType);
        void handleMouseClick(const sf::Vector2i& position);
        
        sf::RenderWindow& getWindow();
        bool isRunning() const;
        const GameState& getCurrentState() const;

        void updateKeyBindingsInfo(KeyBindingsInfo currentKeyBindings);
        const KeyBindingsInfo& getKeyBindingsInfo() const;
};

#endif