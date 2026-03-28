#pragma once
#include <SFML/Graphics.hpp>
#include <filesystem>

namespace fs = std::filesystem;

class StartMenu {
private:
    sf::Font font;
    sf::Text titleText;
    sf::Text newGameText;
    sf::Text loadGameText;
    sf::Text exitText;
    sf::Text noSaveText;
    
    bool saveExists;

public:
    StartMenu();
    bool initialize();
    int show(sf::RenderWindow& window);
    bool checkSaveExists() const { return saveExists; }
};