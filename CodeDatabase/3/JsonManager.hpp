#pragma once
#include <json.hpp>
#include "Serialization/JsonFileManager.hpp"
#include "types.hpp"
#include "LevelManager/LMtypes.hpp"
#include "GameObjects/Spells/Spell.hpp"
#include "InputController/Handlers/sfml/KeysConf.hpp"
#include <unordered_set>


class JsonManager {
private:
    JsonFileManager fileManager;

public:
    void marshal(const MapDTO& map, const char* path);
    void unmarshal(MapDTO& map, const char* path);

    void marshal(const std::vector<TowerDTO>& towers, const char* path);
    void unmarshal(std::vector<TowerDTO>& towers, const char* path);

    void marshal(const std::vector<EnemyDTO>& enemies, const char* path);
    void unmarshal(std::vector<EnemyDTO>& enemies, const char* path);

    void marshal(const PlayerDTO& player, const char* path);
    void unmarshal(PlayerDTO& player, const char* path);

    void marshal(const HandDTO& hand, const char* path);
    void unmarshal(HandDTO& hand, const char* path);

    void marshal(const StatisticDTO& stat, const char* path);
    void unmarshal(StatisticDTO& stat, const char* path);

    void marshal(const std::vector<SpellContract>& traps, const char* path);
    void unmarshal(std::vector<SpellContract>& traps, const char* path);

    void unmarshal(KeysConf& keys, const char* path);
};
