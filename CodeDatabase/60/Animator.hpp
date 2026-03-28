#pragma once
#include <Player.hpp>
#include <Enemy.hpp>
#include <ScreenSize.hpp>
#include <memory>

class Animator {
public:
    Animator(const ScreenSize* const screen, std::shared_ptr<Player> player, std::shared_ptr<Enemy> enemy);

    void playPlayerAttackAnimation();
    bool playEnemyAttackAnimation();

private:
    const ScreenSize* const screen;
    std::shared_ptr<Player> player;
    std::shared_ptr<Enemy> enemy;
};