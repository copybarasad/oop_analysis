#ifndef RENDER_CONTROLLER_H
#define RENDER_CONTROLLER_H

#include "render.h"

template<typename RenderType>
class Render_controller{
private:
    RenderType renderer;
public:
    Render_controller() : renderer(){}

    void render_main_menu(){
        renderer.render_main_menu();
    }

    void render_level(Place& place, Player& player, std::vector<Enemy>& enemies, Tower& tower, MagicTower& magic_tower){
        renderer.render_level(place, player, enemies, tower, magic_tower);
    }

    void render_player_turn(Place& place, Player& player, std::vector<Enemy>& enemies, Tower& tower, MagicTower& magic_tower){
        renderer.render_player_turn(place, player, enemies, tower, magic_tower);
    }
    void render_enemy_turn(Place& place, Player& player, std::vector<Enemy>& enemies, Tower& tower, MagicTower& magic_tower){
        renderer.render_enemy_turn( place, player, enemies, tower, magic_tower);
    }

    void render_upgrade_menu(Player& player){
        renderer.render_upgrade_menu(player);
    }
    void render_upgrade_options(Player& player){
        renderer.render_upgrade_options(player);
    }
    void render_game_over(Game& game, Player& player){
        renderer.render_game_over(game, player);
    }
    void render_level_complete(Game& game){
        renderer.render_level_complete(game);
    }
    void render_game(Game& game, Player& player){
        renderer.render_game(game, player);
    }

    void render_using_spell(Player& player){
        renderer.render_using_spell(player);
    }
    void render_choosing_target(std::vector<Enemy>& enemies, Tower& tower){
        renderer.render_choosing_target(enemies, tower);
    }

};

#endif