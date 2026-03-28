#pragma once
#include "IGameView.h"

class ConsoleView : public IGameView {
public:
    void show_message(const std::string& msg) override;
    void on_level_init(int level) override;
    void on_game_over(bool win) override;
    int ask_upgrade_choice() override;
    int ask_game_over_action() override;
};
