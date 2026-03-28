#pragma once
#include "game_field.h"
#include "player.h"
#include "enemy_manager.h"
#include "include/json.hpp"
#include <string>

using json = nlohmann::json;

class GameSerializer {
public:
    static json PositionToJson(const Position& pos);
    static json GameFieldToJson(const GameField& field);
    static json PlayerToJson(const Player& player);
    static json EnemyManagerToJson(const EnemyManager& em);

    static Position JsonToPosition(const json& j);
    static void JsonToGameField(const json& j, GameField& field);
    static void JsonToPlayer(const json& j, Player& player);
    static void JsonToEnemyManager(const json& j, EnemyManager& em);
    static void ValidateGameState(const json& j);
};
