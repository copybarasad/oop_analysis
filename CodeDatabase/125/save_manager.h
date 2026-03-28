#pragma once
#include "game_field.h"
#include "player.h"
#include "enemy_manager.h"
#include "file_handle.h"
#include "game_serializer.h"
#include <string>

class SaveManager {
public:
    static bool SaveGame(int level, const GameField& field, 
                        const Player& player, const EnemyManager& em);
    static bool LoadGame(int& level, GameField& field, 
                        Player& player, EnemyManager& em);
    static bool SaveExists();

private:
    static constexpr const char* SAVE_FILE = "savegame.json";
};
