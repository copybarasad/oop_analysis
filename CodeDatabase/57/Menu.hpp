#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class Menu {
public:
    Menu(sf::RenderWindow& window);
    ~Menu();
    int show();
    
private:
    sf::RenderWindow& window;
    sf::Font font;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    
    struct MenuItem {
        sf::Text text;
        sf::RectangleShape background;
        sf::RectangleShape border;
    };
    
    std::vector<MenuItem> menuItems;
    int selectedItemIndex;
    
    void draw();
    void moveUp();
    void moveDown();
    void updateButtonAppearance();
};