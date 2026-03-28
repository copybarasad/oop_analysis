#pragma once
#include "managment/game.h"
#include "IGameView.h"

template <typename Renderer>
class GameView : public IGameView
{
    Renderer renderer;

public:
    void drawField(const Field &field) override { renderer.drawField(field); }
    void drawPlayer(const Player &player) override { renderer.drawPlayer(player); }
    void drawEnemies(const std::vector<Position> &enemies) override { renderer.drawEnemies(enemies); }
    void drawLevel(int level) override { renderer.drawLevel(level); }
    void drawGameOver(const Player &player) override { renderer.drawGameOver(player); }
    void drawHand(const Hand &hand) override { renderer.drawHand(hand); }
    void drawImproveOptions() override { renderer.drawImproveOptions(); }
    void showStartMenu() override { renderer.showStartMenu(); }
    void drawSaveDir(std::string folderPath) override { renderer.drawSaveDir(folderPath); }
    void drawSave() override {renderer.drawSave();}

    void update(const Game &game)
    {
        drawPlayer(*game.getPlayer());
        drawLevel(game.getLevel());
        drawField(game.getField());
    }

    void showGameOver(const Game &game)
    {
        drawGameOver(*game.getPlayer());
    }
};
