#pragma once

#include <vector>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Board;
class Player;
class Enemy;
class Ally;
class EnemyTower;
class EnemyBase;
class LevelManager;
enum class Difficulty;

json BuildSaveJson(
    const Board& board,
    const Player& player,
    const std::vector<Enemy>& enemies,
    const std::vector<Ally>& allies,
    const std::vector<EnemyTower>& towers,
    const EnemyBase* base,
    const LevelManager& level_manager,
    Difficulty difficulty
);

void ApplySaveJson(
    const json& root,
    Board& board,
    Player& player,
    std::vector<Enemy>& enemies,
    std::vector<Ally>& allies,
    std::vector<EnemyTower>& towers,
    EnemyBase*& base,
    LevelManager& level_manager,
    Difficulty& difficulty,
    int& settings_board_width,
    int& settings_board_height
);