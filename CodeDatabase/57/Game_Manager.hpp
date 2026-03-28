#pragma once
#include <SFML/Graphics.hpp>
#include "Field/Field.hpp"
#include "Entity_Manager.hpp"
#include "Systems/Render_System.hpp"
#include "HeroController.hpp"
#include "Spells/SpellHand.hpp"


enum game_state {
    game_running,
    victory,
    loss,
    level_complete,
    game_over_dialog,
    level_up_dialog
};
 
class Game_Manager {
private:
    Field* field;
    Entity_Manager* entity_manager;
    Render_System* render_system;
    sf::RenderWindow* window;
    GameLogger* logger_consol;
    HeroController* hero_controller;
    game_state status;
    int counter;
    int current_level = 1;
    bool show_restart_dialog = false;
    sf::Text levelText;
    sf::Font levelFont;
    
    void initLevelText();
    void updateLevelText();

    void renderDefeatDialog();
    void renderLevelCompleteDialog();
    void handleDialogEvents(sf::Event& event);

public:
    Game_Manager();
    ~Game_Manager();
    
    void run();
    void startNewGame(sf::Vector2u size = sf::Vector2u(15, 15), int startLevel = 1);
    void loadGame(const std::string& filename = "save.json");
    void handleEvents();
    void update();
    void render();
    void restartGame();
    void nextLevel();
    void showLevelUpDialog();
    void renderLevelUpDialog();
};