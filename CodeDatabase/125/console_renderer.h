#pragma once
#include "renderer_interface.h"

class ConsoleRenderer : public IRenderer {
public:
    void RenderField(const GameField& field, const Player& player, const EnemyManager& em) override;
    void RenderStatus(const Player& player, const EnemyManager& em) override;
    void RenderWelcome() override;
    void RenderLevelStart(int level, int enemy_count) override;
    void RenderGameResult(const GameField& field, const Player& player, const EnemyManager& em) override;
};
