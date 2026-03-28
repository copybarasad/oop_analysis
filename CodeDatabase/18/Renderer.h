#pragma once
#include <SFML/Graphics.hpp>
#include <optional>
#include "GameLogic.h"

class SfmlRenderer {
private:
    sf::RenderWindow window;
    sf::Texture blockedTex, emptyTex, buildingTex, enemyTex;
    sf::Font font;

    const float tileSize = 25.f;
    const float spacing = 2.f;

    void drawField(const Field& field, int selectedSpellIndex);
    void drawUI(const Player& player, int selectedSpellIndex);
    void drawMenu();
    void drawLevelUp();
    void drawGameOver();
    void drawVictory();

public:
    SfmlRenderer();

    bool isOpen() const { return window.isOpen(); }
    void close() { window.close(); }

    std::optional<sf::Event> pollEvent() { return window.pollEvent(); }

    sf::Window& getWindow() { return window; }

    void render(const GameLogic& game, int selectedSpellIndex);
};