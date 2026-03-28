#pragma once
#include <BattleController.hpp>
#include <ncurses.h>
#include <Player.hpp>
#include <Enemy.hpp>
#include <string>
#include <vector>
#include <memory>

class BattleUI {
public:
    BattleUI(const ScreenSize* const screen, std::shared_ptr<Player> player, std::shared_ptr<Enemy> enemy, int& selected_option, BattleController::BattleTurn& turn);

    const std::vector<std::string>& getOptions() const;
    void drawPlayerBar();
    void updateBattleWindows();
    void drawBattleScreen();

    ~BattleUI();
    
private:
    const ScreenSize* const screen;    
    WINDOW* pl_bar;
    WINDOW* en_bar;
    WINDOW* act_bar;
    std::shared_ptr<Player> player;
    std::shared_ptr<Enemy> enemy;
    std::vector<std::string> options;
    int& selected_option;
    BattleController::BattleTurn& turn;
};