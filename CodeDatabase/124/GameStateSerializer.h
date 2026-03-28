#ifndef GAMESTATESERIALIZER_H
#define GAMESTATESERIALIZER_H

#include "GameState.h"
#include "json.hpp"
#include <string>

class GameStateSerializer {
public: 
    static nlohmann::json toJson(const GameState& state);
    static GameState fromJson(const nlohmann::json& j);
     
    static std::string toString(const GameState& state);
    static GameState fromString(const std::string& jsonStr);

private:
    static void serializeObstacles(nlohmann::json& j, const GameState& state);
    static void serializeTowers(nlohmann::json& j, const GameState& state);
    static void serializeTraps(nlohmann::json& j, const GameState& state);
    static void serializeSpells(nlohmann::json& j, const GameState& state);
    
    static void deserializeObstacles(const nlohmann::json& j, GameState& state);
    static void deserializeTowers(const nlohmann::json& j, GameState& state);
    static void deserializeTraps(const nlohmann::json& j, GameState& state);
    static void deserializeSpells(const nlohmann::json& j, GameState& state);
};

#endif