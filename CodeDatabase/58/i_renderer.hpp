#pragma once

#include "game.hpp"

class IRenderer{
public:
    virtual ~IRenderer() = default;

    virtual void render_world(World* world) = 0;
    virtual void render_player_stats(Player& player, int level) = 0;
    virtual void render_player_hand(Player& player) = 0;
    virtual void render_legend() = 0;
    virtual void render_help(std::unordered_map<std::string, CommandType> keybindings) = 0;
    virtual void render_full_ui(Game& game) = 0;
    virtual void render_msg(std::string msg) = 0;
    virtual void render_final_output(bool is_win, int points) = 0;
    virtual void render_bind_keys(std::unordered_map<std::string, CommandType>& keybindings) = 0;
};