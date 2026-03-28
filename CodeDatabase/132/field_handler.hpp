#pragma once

#include <map>
#include <set>
#include <stdexcept>
#include <functional>

#include "field_image.hpp"
#include "field.hpp"
// #include "game_object.hpp"

#include "ally.hpp"
#include "player.hpp"
#include "enemy.hpp"
#include "enemy_tower.hpp"
#include "enemy_spawner.hpp"

#include "trap.hpp"

#include "stone.hpp"
#include "swamp.hpp"

class FieldHandler
{
private:
    std::map<char, std::set<Position>> pos_container;

    Field landscape;
    Field field;
public:
    FieldHandler(int h, int w);
    FieldHandler(FieldImage& img);
    FieldImage get_data_image();

    int width() const;
    int height() const;

    void place_img(const Position &pos, const MobImage &img, char type);
    void place_img(const Position &pos, const PlayerImage &img);
    void place_img(const Position &pos, const BuildingImage &img, char type);
    void place_img(const Position &pos, const TrapImage &img);

    void place_landscape_obj(const Position &pos, GameObject *obj);
    void place_obj(const Position &pos, GameObject *obj);
    GameObject* remove_obj(const Position &pos);
    void move_obj(const Position &cur, const Position &new_pos);

    std::set<Position> get_positions(char type) const;
    int count_positions(char type) const;
    GameObject* get_obj(const Position& pos);

    Player* get_player();

    std::vector<Position> free_cells(const Position &top_left, const Position &bottom_right) const;

    const Field& get_field() const;
    // Field& get_field();

    Position get_nearest(const Position&to, std::set<char> search_cells) const;
};