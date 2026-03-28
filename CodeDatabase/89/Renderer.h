#ifndef RENDERER_H
#define RENDERER_H

#include "GameGrid.h"
#include "Player.h"
#include <vector>
#include <memory>

class IRenderer {
public:
    virtual ~IRenderer() = default;
    virtual void render(const GameGrid& grid, const Player* player, 
                       size_t enemyCount, size_t towerCount) = 0;
    virtual void renderMainMenu() = 0;
    virtual void renderGameOver() = 0;
    virtual void renderUpgradeMenu(const Player& player) = 0;
    virtual void renderSpells(const Player& player) = 0;
};

class ConsoleRenderer : public IRenderer {
public:
    void render(const GameGrid& grid, const Player* player, 
                size_t enemyCount, size_t towerCount) override;
    void renderMainMenu() override;
    void renderGameOver() override;
    void renderUpgradeMenu(const Player& player) override;
    void renderSpells(const Player& player) override;
};

#endif