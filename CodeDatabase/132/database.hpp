#pragma once

#include <pqxx/pqxx>
#include <iostream>
#include <string>

#include "game_image.hpp"

class DataBase {
private:
    std::string db_name;
    pqxx::connection c;

    void create_tables_if_not_exist();
    
    void save_players(pqxx::work& tx, const std::map<Position, PlayerImage>& players, int game_id);
    void save_mobs(pqxx::work& tx, const std::map<Position, MobImage>& mobs, const std::string& type, int game_id);
    void save_buildings(pqxx::work& tx, const std::map<Position, BuildingImage>& buildings, const std::string& type, int game_id);
    void save_traps(pqxx::work& tx, const std::map<Position, TrapImage>& traps, int game_id);
    void save_landscape(pqxx::work& tx, const std::vector<Position>& positions, const std::string& type, int game_id);
    void save_effects(pqxx::work& tx, const std::vector<EffectImage>& effects, const std::string& entity_type, int entity_id, int game_id);

    void load_players(pqxx::work& tx, FieldImage& field, int game_id);
    void load_mobs(pqxx::work& tx, FieldImage& field, const std::string& type, int game_id);
    void load_buildings(pqxx::work& tx, FieldImage& field, const std::string& type, int game_id);
    void load_traps(pqxx::work& tx, FieldImage& field, int game_id);
    void load_landscape(pqxx::work& tx, FieldImage& field, const std::string& type, int game_id);
    std::vector<EffectImage> load_effects(pqxx::work& tx, const std::string& entity_type, int entity_id, int game_id);

public:
    DataBase(std::string db_name);
    bool save_exists(const std::string &save_name);
    bool save_game(const std::string& save_name, const GameImage& game_image);
    GameImage* load_game(const std::string& save_name);
};