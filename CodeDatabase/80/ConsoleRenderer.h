// ConsoleRenderer.h
#pragma once
#ifndef CONSOLERENDERER_H
#define CONSOLERENDERER_H

#include "GameRenderer.h"

class ConsoleRenderer : public GameRenderer {
private:
    int screenWidth;

    // ?¡ô????ÿ¢??¬?«? ??¢?ý« ý?¯ ?¢ ð¡?÷òð òÿ ¢«
    void renderMap(const GameRenderData& data) const;
    void renderMapSimple(const GameRenderData& data) const;
    void renderMapLegend() const;

    std::string centerText(const std::string& text, int width) const;
    void renderMainMenu(const GameRenderData& data);
    void renderGameplay(const GameRenderData& data);
    void renderLevelComplete(const GameRenderData& data);
    void renderGameEnd(const GameRenderData& data);
    void renderLoadMenu(const GameRenderData& data);
    void renderSaveMenu(const GameRenderData& data);

public:
    ConsoleRenderer(int width = 80);
    bool initialize() override;
    void clearScreen() override;
    void render(const GameRenderData& data) override;
};

#endif // CONSOLERENDERER_H