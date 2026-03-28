#pragma once

#include "i_renderer.hpp"
#include <iostream>

class ConsoleRenderer : public IRenderer{
    void render_world(World* world) override;
    void render_player_stats(Player& player, int level) override;
    void render_player_hand(Player& player) override;
    void render_legend() override;
    void render_help(std::unordered_map<std::string, CommandType> keybindings) override;
    void render_full_ui(Game& game) override;
    void render_msg(std::string msg) override;
    void render_final_output(bool is_win, int points) override;
    void render_bind_keys(std::unordered_map<std::string, CommandType>& keybindings) override;
};