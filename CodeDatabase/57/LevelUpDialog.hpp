#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "./Entities/Hero.hpp"

class LevelUpDialog {
public:
    LevelUpDialog(sf::RenderWindow& window, Hero* hero);
    int show();
    
private:
    sf::RenderWindow& window;
    Hero* hero;
    sf::Font font;
    sf::Texture heroTexture;
    sf::Sprite heroSprite;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    
    struct UpgradeOption {
        sf::Text text;
        sf::RectangleShape background;
        sf::RectangleShape border;
        std::string description;
    };
    
    std::vector<UpgradeOption> options;
    int selectedOptionIndex;
    
    void loadResources();
    void createOptions();
    void draw();
    void moveUp();
    void moveDown();
    void updateOptionAppearance();
    void applyUpgrade();
};