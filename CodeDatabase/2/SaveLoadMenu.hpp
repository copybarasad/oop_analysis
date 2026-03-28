#pragma once
#include <SFML/Graphics.hpp>
#include "SaveManager.hpp"

class SaveLoadMenu {
private:
    sf::Font font;
    SaveManager saveManager;
    bool isSaveMenu; // true для сохранения, false для загрузки
    
    sf::Text titleText;
    std::vector<sf::Text> slotTexts;
    sf::Text backText;

public:
    SaveLoadMenu(bool isSaveMenu = false);
    bool initialize();
    int show(sf::RenderWindow& window);
    void setMode(bool saveMode) { isSaveMenu = saveMode; }
};