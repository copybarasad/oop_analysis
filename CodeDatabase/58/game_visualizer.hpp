#pragma once

#include "i_renderer.hpp"
#include "game.hpp"

template<typename Render>
class GameVisualizer{
private:
    Game* game;
    IRenderer* render;
public:
    GameVisualizer(Game* game) : game(game), render(new Render) {};
    ~GameVisualizer() {delete render;};

    IRenderer* get_renderer() {return render;};

    void render_world(World* world) {render->render_world(world);};
    void render_player_stats(Player& player, int level) {render->render_player_stats(player, level);};
    void render_player_hand(Player& player) {render->render_player_hand(player);};
    void render_legend() {render->render_legend();};
    void render_help(std::unordered_map<std::string, CommandType> keybindings) {render->render_help(keybindings);};
    void render_full_ui(Game& game) {render->render_full_ui(game);};
    void render_msg(std::string msg) {render->render_msg(msg);};
    void render_final_output(bool is_win, int points) {render->render_final_output(is_win, points);};
};