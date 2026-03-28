#ifndef CONSOLERENDERER_H
#define CONSOLERENDERER_H

#include "Renderer.h"
#include "GameState.h"
#include "CellContent.h"
#include <sstream>
#include <string>

class ConsoleRenderer : public Renderer {
private:
    void updateFieldContent(GameField& field, const GameState& gameState) const;
    std::string getGameStateString(const GameState& gameState) const;

public:
    ConsoleRenderer() = default;

    void render(const GameState& gameState) const override;
    void showMessage(const std::string& message) const override;
};

#endif