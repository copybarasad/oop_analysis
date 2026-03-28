#ifndef GAMERENDERER_H
#define GAMERENDERER_H

#include "IGameRenderer.h"
#include <vector>
#include <string>

class GameRenderer : public IGameRenderer {
public:
    void renderStats(const Player& player, int turnCount, const std::vector<Enemy>& enemies,
        const std::vector<EnemyTower>& towers, const GameField& field) override;

    void renderField(const GameField& field, const Player& player,
        const std::vector<Enemy>& enemies, const std::vector<EnemyTower>& towers) override;

    void renderGameStart() override;
    void renderVictory(int turnCount, int score) override;
    void renderGameOver(int turnCount, int score) override;
    void renderInvalidCommand() override;
    void renderControls() override;
    void renderSpellInfo(const Player& player) override;

private:
    std::string getHealthStatus(char symbol, int health, int maxHealth) const;
};

#endif // GAMERENDERER_H