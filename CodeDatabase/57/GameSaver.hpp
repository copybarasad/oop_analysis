#pragma once
#include "./Entities/Hero.hpp"
#include "Entity_Manager.hpp"
#include <nlohmann/json.hpp>
#include <fstream>
#include "./Field/Field.hpp"
using json = nlohmann::json;

class GameSaver {
public:
    static void saveGame(Hero* hero, Entity_Manager* manager, Field* field, const std::string& filename, int current_level);
    static void loadGame(Entity_Manager*& manager, Field*& field, const std::string& filename, int& current_level);
};