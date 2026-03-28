#pragma once
#include <../nlohmann/json.hpp>
#include <memory>
#include <vector>
#include "SaveLoadExceptions.h"

class GameWorld;
class GameField;
class Cell;
class Character;
class Player;
class PlayerHand;
class ISpell;
class FieldGenerator;

class Serializer {
public:
    static nlohmann::json serializeGameWorld(const GameWorld& world, const FieldGenerator& generator);
    static std::unique_ptr<GameWorld> deserializeGameWorld(const nlohmann::json& j);

private:
    static nlohmann::json serializeCell(const Cell& cell);
    static Cell deserializeCell(const nlohmann::json& j);
    
    static nlohmann::json serializeGameField(const GameField& field);
    static GameField deserializeGameField(const nlohmann::json& j);
    
    static nlohmann::json serializePlayerHand(const PlayerHand& hand);
    static void deserializePlayerHand(PlayerHand& hand, const nlohmann::json& j);
    
    static nlohmann::json serializePlayer(const Player& player);
    static void deserializePlayer(Player& player, const nlohmann::json& j);
    
    static nlohmann::json serializeObjectPositions(const GameWorld& world);
    static void deserializeObjectPositions(GameWorld& world, FieldGenerator& generator, const nlohmann::json& j);

    static std::unique_ptr<ISpell> createSpellByName(const std::string& name, int level);
    static int getSpellIndexByName(const std::string& name);

    static void validateGameWorldJson(const nlohmann::json& j);
    static void validateFieldJson(const nlohmann::json& j);
    static void validatePlayerJson(const nlohmann::json& j);
    static void validateObjectPositionsJson(const nlohmann::json& j);
};