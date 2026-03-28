#pragma once

#include "command.h"
#include "enemy.h"
#include "enemy_tower.h"
#include "game_state.h"
#include "grid.h"
#include "invalidSaveDataException.h"
#include "loadGameException.h"
#include "player.h"
#include "saveGameException.h"

#include <memory>
#include <string>
#include <string_view>
#include <vector>

class Game {
public:
    Game();

    void Step(const Command& command);

    GameState state() const { return currentState; }
    int level() const { return currentLevel; }

    const Grid* grid() const { return gameGrid.get(); }
    Grid* grid() { return gameGrid.get(); }

    const Player* player() const { return playerCharacter.get(); }
    Player* player() { return playerCharacter.get(); }

    const std::vector<std::unique_ptr<Enemy>>& enemies() const { return enemiesList; }
    const EnemyTower* tower() const { return enemyTower.get(); }

    std::string_view last_message() const { return lastMessage; }
    void ClearLastMessage() { lastMessage.clear(); }

private:
    void StartNewGame();
    void StartLevel(int level);
    void ResetGameState();
    void PrepareNextLevel();

    void ApplyLevelUp(CommandType commandType);
    bool ApplyPlayingCommand(const Command& command);

    void HandleEnemiesTurn();
    void HandleTowerTurn();

    bool CheckLevelCompleted() const;
    bool IsGameOver() const;
    Point* FindAvailablePosition(int centerX, int centerY, int searchRadius) const;

    unsigned long long CalculateHash(const std::string& data) const;
    void SaveGame(std::string_view fileName);
    void LoadGameFromFile(std::string_view fileName);

    void SetMessage(std::string message) { lastMessage = std::move(message); }

    GameState currentState;
    std::unique_ptr<Grid> gameGrid;
    std::unique_ptr<Player> playerCharacter;
    std::vector<std::unique_ptr<Enemy>> enemiesList;
    std::unique_ptr<EnemyTower> enemyTower;

    int currentLevel;
    bool towerAttackToggle = false;

    std::string lastMessage;

    static constexpr int DEFAULT_GRID_SIZE = 10;
    static constexpr int MAXIMUM_LEVELS = 3;
    static constexpr const char* SAVE_FILE_NAME = "game_save.dat";
};
