#ifndef IGAMERENDERER_H
#define IGAMERENDERER_H

#include "Player.h"
#include "GameField.h"
#include "Enemy.h"
#include "EnemyTower.h"
#include <vector>

class IGameRenderer {
public:
    virtual ~IGameRenderer() = default;

    virtual void renderStats(const Player& player, int turnCount,
        const std::vector<Enemy>& enemies,
        const std::vector<EnemyTower>& towers,
        const GameField& field) = 0;

    virtual void renderField(const GameField& field, const Player& player,
        const std::vector<Enemy>& enemies,
        const std::vector<EnemyTower>& towers) = 0;

    virtual void renderSpellInfo(const Player& player) = 0;
    virtual void renderGameStart() = 0;
    virtual void renderVictory(int turnCount, int score) = 0;
    virtual void renderGameOver(int turnCount, int score) = 0;
    virtual void renderInvalidCommand() = 0;
    virtual void renderControls() = 0;
};

#endif // IGAMERENDERER_H