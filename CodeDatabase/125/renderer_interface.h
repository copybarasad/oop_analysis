#pragma once
#include "game_field.h"
#include "player.h"
#include "enemy_manager.h"

class IRenderer {
public:
    virtual ~IRenderer() = default;
    
    virtual void RenderField(const GameField& field, const Player& player, const EnemyManager& em) = 0;
    virtual void RenderStatus(const Player& player, const EnemyManager& em) = 0;
    virtual void RenderWelcome() = 0;
    virtual void RenderLevelStart(int level, int enemy_count) = 0;
    virtual void RenderGameResult(const GameField& field, const Player& player, const EnemyManager& em) = 0;
};
