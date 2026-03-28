#include "GameStateSerializer.h"
#include "GameExceptions.h"

using json = nlohmann::json;

json GameStateSerializer::toJson(const GameState& state) {
    json j;
     
    j["level"] = state.level;
    j["player_health"] = state.playerHealth;
    j["player_score"] = state.playerScore;
    j["player_mana"] = state.playerMana;
    j["enemies_defeated"] = state.enemiesDefeated;
    j["enemy_health"] = state.enemyHealth;
    j["enemy_level"] = state.enemyLevel;
     
    j["field_width"] = state.fieldWidth;
    j["field_height"] = state.fieldHeight;
    j["player_x"] = state.playerX;
    j["player_y"] = state.playerY;
    j["enemy_x"] = state.enemyX;
    j["enemy_y"] = state.enemyY;
     
    serializeObstacles(j, state);
    serializeTowers(j, state);
    serializeTraps(j, state);
    serializeSpells(j, state);
    
    return j;
}

void GameStateSerializer::serializeObstacles(json& j, const GameState& state) {
    j["obstacles"] = json::array();
    for (const auto& obstacle : state.obstacles) {
        j["obstacles"].push_back({{"x", obstacle.first}, {"y", obstacle.second}});
    }
}

void GameStateSerializer::serializeTowers(json& j, const GameState& state) {
    j["towers"] = json::array();
    for (const auto& tower : state.towers) {
        j["towers"].push_back({
            {"x", std::get<0>(tower)},
            {"y", std::get<1>(tower)}, 
            {"health", std::get<2>(tower)}
        });
    }
}

void GameStateSerializer::serializeTraps(json& j, const GameState& state) {
    j["traps"] = json::array();
    for (const auto& trap : state.traps) {
        j["traps"].push_back({
            {"x", std::get<0>(trap)},
            {"y", std::get<1>(trap)},
            {"damage", std::get<2>(trap)},
            {"active", std::get<3>(trap)}
        });
    }
}

void GameStateSerializer::serializeSpells(json& j, const GameState& state) {
    j["spells"] = json::array();
    for (const auto& spell : state.spells) {
        j["spells"].push_back({
            {"name", spell.first},
            {"cooldown", spell.second}
        });
    }
}

GameState GameStateSerializer::fromJson(const nlohmann::json& j) {
    GameState state;
     
    state.level = j.value("level", 1);
    state.playerHealth = j.value("player_health", 100);
    state.playerScore = j.value("player_score", 0);
    state.playerMana = j.value("player_mana", 0);
    state.enemiesDefeated = j.value("enemies_defeated", 0);
    state.enemyHealth = j.value("enemy_health", 0);
    state.enemyLevel = j.value("enemy_level", 1);
     
    state.fieldWidth = j.value("field_width", 10);
    state.fieldHeight = j.value("field_height", 10);
    state.playerX = j.value("player_x", 0);
    state.playerY = j.value("player_y", 0);
    state.enemyX = j.value("enemy_x", 0);
    state.enemyY = j.value("enemy_y", 0);
     
    deserializeObstacles(j, state);
    deserializeTowers(j, state);
    deserializeTraps(j, state);
    deserializeSpells(j, state);
    
    return state;
}

void GameStateSerializer::deserializeObstacles(const json& j, GameState& state) {
    if (j.contains("obstacles") && j["obstacles"].is_array()) {
        for (const auto& obstacle : j["obstacles"]) {
            state.obstacles.push_back({
                obstacle.value("x", 0),
                obstacle.value("y", 0)
            });
        }
    }
}

void GameStateSerializer::deserializeTowers(const json& j, GameState& state) {
    if (j.contains("towers") && j["towers"].is_array()) {
        for (const auto& tower : j["towers"]) {
            state.towers.push_back(std::make_tuple(
                tower.value("x", 0),
                tower.value("y", 0),
                tower.value("health", 0)
            ));
        }
    }
}

void GameStateSerializer::deserializeTraps(const json& j, GameState& state) {
    if (j.contains("traps") && j["traps"].is_array()) {
        for (const auto& trap : j["traps"]) {
            state.traps.push_back(std::make_tuple(
                trap.value("x", 0),
                trap.value("y", 0),
                trap.value("damage", 0),
                trap.value("active", true)
            ));
        }
    }
}

void GameStateSerializer::deserializeSpells(const json& j, GameState& state) {
    if (j.contains("spells") && j["spells"].is_array()) {
        for (const auto& spell : j["spells"]) {
            state.spells.push_back(std::make_pair(
                spell.value("name", ""),
                spell.value("cooldown", 0)
            ));
        }
    }
}

std::string GameStateSerializer::toString(const GameState& state) {
    return toJson(state).dump(4);  
}

GameState GameStateSerializer::fromString(const std::string& jsonStr) {
    try {
        return fromJson(json::parse(jsonStr));
    } catch (const json::parse_error& e) {
        throw SaveException("JSON parse error: " + std::string(e.what()));
    }
}