#ifndef GAME_GUI
#define GAME_GUI

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <memory>
#include <iostream>
#include <sstream>

#include "gameManager.hpp"

#include "../headers/saveManager.hpp"
#include "gameState.hpp"
#include "textureManager.hpp"
#include "enemyInfoWindow.hpp"
#include "keyBindingsInfo.hpp"

class GameGUI {
    private:
        sf::RenderWindow window;
        TextureManager textureManager;
        EnemyInfoWindow enemyInfoWindow;
        
        sf::Font font;
        sf::Text pointsText;
        sf::Text healthText;
        sf::Text manaText;
        sf::Text levelText;
        sf::Text messageText;
        
        const int CELL_SIZE = 40;
        const int SIDEBAR_WIDTH = 350;

        void loadTextures();
        void setupUI();

        void renderMainMenu(const KeyBindingsInfo& keyBindings);
        void renderPlaying(const GameState& state, const GameManager* gameManager, int currentLevel, const KeyBindingsInfo& keyBindings);
        void renderLevelComplete(const KeyBindingsInfo& keyBindings);
        void renderGameOver(const KeyBindingsInfo& keyBindings);
        void renderUpgrading(const KeyBindingsInfo& keyBindings);
        void renderBuyingSpell(const KeyBindingsInfo& keyBindings);
        
        void renderField(const GameManager* gameManager);
        void renderSidebar(const GameManager* gameManager, int currentLevel, const KeyBindingsInfo& keyBindings);
        void renderSpells(const GameManager* gameManager, float startY, const KeyBindingsInfo& keyBindings);
        void renderAttackRadius(const GameManager* gameManager);
        void renderSpellRadius(const GameManager* gameManager);
        void renderEnemyInfo(const GameState& state);
        void renderMenuOptions(const std::vector<std::string>& options, float startY, const KeyBindingsInfo& keyBindings);

    public:
        GameGUI();
        void render(const GameState& state, const GameManager* gameManager, int currentLevel, const KeyBindingsInfo& keyBindings);

        sf::RenderWindow& getWindow();
};

#endif