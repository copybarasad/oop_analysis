#pragma once
#include "Renderer.h"
#include "GameView.h"
#include <SFML/Graphics.hpp>

// Реализация Renderer для SFML (использует GameView)
class SFMLRenderer : public Renderer {
private:
    GameView gameView_;

public:
    SFMLRenderer(sf::RenderWindow& window, GameModel& model)
        : gameView_(window, model) {}
    
    void render(float dt) override {
        gameView_.render(dt);
    }
};

