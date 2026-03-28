#pragma once
#include <iostream>
#include <string>

class Game;

class Renderer {
public:
    virtual ~Renderer() = default;
    virtual void render(const Game& game) = 0;
    virtual void showMessage(const std::string& message) = 0;
};