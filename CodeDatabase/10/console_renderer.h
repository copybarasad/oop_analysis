#pragma once
#include "renderer.h"

class ConsoleRenderer : public Renderer {
public:
    void render(const Game& game) override;
    void showMessage(const std::string& message) override;
};