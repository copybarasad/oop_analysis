#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "InterfaceGameLogger.hpp"

class GameLogger: public InterfaceGameLogger {
private:
    sf::Text text;
    sf::Font font;
    std::vector<std::string> messages;
    sf::Texture texture;
    sf::Sprite sprite;
    sf::Vector2i const LOCATION_SWITOC = {0, 0};
    sf::Vector2i const LOCATION_TEXT = {50, 45};
    int const SIZE_TEXT = 20;
    sf::Color const COLOR_TEXT = {87, 83, 71};

public:
    GameLogger();
    void log(const std::string& message) override;
    void draw(sf::RenderWindow& window);
};