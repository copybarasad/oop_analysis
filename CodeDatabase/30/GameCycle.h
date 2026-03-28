#pragma once
#include "Enums.h"
#include "Position.h"
#include "Manager.h"
#include "PlayerCommand.h"
#include <optional>
#include <vector>
#include <algorithm>
#include <random>

// Класс игрового цикла
class GameCycle {
public:
    GameCycle();

    void startGame();
    void setDimensions(int rows, int cols);
    bool step(const PlayerCommand& cmd);

    enum class GameState {
        Running,
        WaitingForUpgrade,
        GameOver
    };

    bool isRunning() const { return state_ == GameState::Running || state_ == GameState::WaitingForUpgrade; }
    bool isLevelOver() const { return isWin() || isLose(); }
    bool isWin() const;
    bool isLose() const;
    GameState state() const { return state_; }

    void upgradeHealth();
    void upgradeDamage();
    void proceedToNextLevel();

    const Manager& manager() const { return *manager_; }
    Manager& manager() { return *manager_; }
    std::optional<Position> playerPos() const;
    EntityId playerId() const { return playerId_; }

    int currentLevel() const { return level_; }

    void saveGame(const std::string& filename);
    void loadGame(const std::string& filename);

private:
    void initLevel(int rows, int cols, std::unique_ptr<Player> player = nullptr);
    void nextLevel();
    void playerTurn_(const PlayerCommand& cmd);
    void alliesTurn_();
    void enemiesTurn_();
    void buildingsTurn_();

    template <typename T, typename... Args>
    void spawnEntities(int count, Args&&... args) {
        std::vector<Position> freeCells;
        int rows = manager_->rows();
        int cols = manager_->cols();
        
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                Position p{r, c};
                if (manager_->field().cellAt(p).isPassable() && !manager_->occupantAt(p)) {
                    freeCells.push_back(p);
                }
            }
        }

        std::random_device rd;
        std::mt19937 gen(rd());
        std::shuffle(freeCells.begin(), freeCells.end(), gen);

        int spawned = 0;
        for (const auto& pos : freeCells) {
            if (spawned >= count) break;
            if (manager_->entities().spawnAt(std::make_unique<T>(args...), pos) != InvalidEntityId) {
                spawned++;
            }
        }
    }

private:
    std::unique_ptr<Manager> manager_;
    EntityId playerId_{InvalidEntityId};
    GameState state_{GameState::Running};
    int level_{1};
    int initialRows_{0};
    int initialCols_{0};
};
