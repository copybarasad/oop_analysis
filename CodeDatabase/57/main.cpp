#include <SFML/Graphics.hpp>
#include <filesystem>
#include <vector>
#include <string>
#include <algorithm>
#include "Entities/Portal.hpp"
#include "Entities/Slime.hpp"
#include "Entities/Demon.hpp"
#include "Entities/Shadow.hpp"  
#include <random>
#include "Managers/Game_Manager.hpp"
#include <iostream>
#include "Managers/Menu.hpp"

int main() {
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    sf::RenderWindow menuWindow(desktop, "Dungeon Adventure", sf::Style::Fullscreen);
    
    Menu menu(menuWindow);
    int choice = menu.show();
    
    menuWindow.close();
    
    Game_Manager game;
    
    switch (choice) {
        case 0:
            game.startNewGame();
            break;
        case 1:
            game.loadGame();
            break;
        case 2:
        default:
            return 0;
    }
    
    game.run();
    
    return 0;
}