#include <SFML/Graphics.hpp>
#include "Game.h"
#include "GameMenu.h"
#include "Architecture.h"
#include "InputConfig.h"
#include "GameLevel.h" 
#include "SaveManager.h"
#include "GameExceptions.h"
#include <ctime>
#include <iostream>

int main() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    sf::RenderWindow window(sf::VideoMode({1280, 720}), "Magical Battle Architect", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(120);

    // ЗАГРУЗКА УПРАВЛЕНИЯ
    auto keyBindings = InputConfig::loadBindings("keys.cfg");

    while (window.isOpen()) {
        GameMenu menu(window);
        GameMenu::Action action = menu.run();

        if (action == GameMenu::Action::EXIT) {
            window.close();
            break;
        }
        
        // Настройка уровня
        int levelIdx = 1;
        GameMode mode = GameMode::STORY;
        GameLevel::LevelType lType = GameLevel::LevelType::FOREST;
        bool load = false;

        if (action == GameMenu::Action::LOAD_GAME) load = true;
        if (action == GameMenu::Action::FREE_PLAY) {
            mode = GameMode::FREE_PLAY;
            levelIdx = menu.getSelectedLevel();
            if (levelIdx >= 4) lType = GameLevel::LevelType::DUNGEON;
            if (levelIdx >= 7) lType = GameLevel::LevelType::CASTLE;
            if (levelIdx == 10) lType = GameLevel::LevelType::BOSS_ARENA;
        }

        GameLevel level(lType, levelIdx);
        
        Game game(level.getConfig(), mode, levelIdx);
        
        if (load) {
             try { SaveManager::loadGame(game); } 
             catch (const std::exception& e) { std::cerr << e.what() << std::endl; }
        }

        // Передаем bindings в конструктор
        SfmlInputHandler input(window, keyBindings);

        GameVisualizer<GameRender> visualizer(window);

        GameController controller(game, input, visualizer, window);
        
        controller.run();
    }

    return 0;
}
