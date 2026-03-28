#pragma once
#include <Player.hpp>
#include <Enemy.hpp>
#include <ScreenSize.hpp>
#include <Ceil.hpp>
#include <vector>
#include <Animator.hpp>
#include <memory>

class BattleUI;

class BattleController {
public:
    enum class BattleTurn {
        PLAYER,
        ENEMY
    };

    BattleController(const ScreenSize* const screen, std::vector<std::vector<Ceil>>& ceils, std::shared_ptr<Player> player, std::shared_ptr<Enemy> enemy, BattleTurn turn, int field_height, int field_width);
    BattleTurn handlePlayerTurn();
    BattleTurn handleEnemyTurn();
    ~BattleController();

    bool start();
    
private:
    const ScreenSize* const screen;
    std::vector<std::vector<Ceil>>& ceils;
    std::shared_ptr<Player> player;
    std::shared_ptr<Enemy> enemy;
    BattleTurn turn;
    int selected_option;
    Animator animator;
    std::unique_ptr<BattleUI> ui;
    bool battleIsOver;
    int battleResult;
    int field_height;
    int field_width;
};