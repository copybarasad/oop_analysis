#pragma once

#include <SFML/Graphics.hpp>
#include "GameModel.h"

class AbstractGameView {
private:
    sf::RenderWindow& window_;
    GameModel& model_;
public:
    AbstractGameView(
        sf::RenderWindow& window,
        GameModel& model
    ): window_(window), model_(model) {};

    ~AbstractGameView() = default;

    virtual void render(float dt) = 0;
};