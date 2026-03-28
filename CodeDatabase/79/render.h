#ifndef RENDER_H
#define RENDER_H

#include <vector>

class Game;
class Place;
class Player;
class Enemy;
class Tower;
class MagicTower;

class Render{
public:
    void render_main_menu();
    void render_level(Place& place, Player& player, std::vector<Enemy>& enemies, Tower& tower, MagicTower& magic_tower);
    void render_player_turn(Place& place, Player& player, std::vector<Enemy>& enemies, Tower& tower, MagicTower& magic_tower);
    void render_enemy_turn(Place& place, Player& player, std::vector<Enemy>& enemies, Tower& tower, MagicTower& magic_tower);
    void render_upgrade_menu(Player& player);
    void render_upgrade_options(Player& player);
    void render_game_over(Game& game, Player& player);
    void render_level_complete(Game& game);
    void render_game(Game& game, Player& player);
    void render_using_spell(Player& player);
    void render_choosing_target(std::vector<Enemy>& enemies, Tower& tower);

    void render_place(Place& place);
    void render_player_stats(Player& player);
    void render_enemy_stats(std::vector<Enemy>& enemies, Tower& tower, MagicTower& magic_tower);
};

#endif