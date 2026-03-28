#pragma once
#include <memory>
#include "GameField.h"
#include "Player.h"
#include "SaveManager.h"


enum class GameState {
    Running,
    LevelCompleted,
    GameOver
};

class Game {
    GameField field_;
    std::shared_ptr<Player> player_;
    int level_ = 1;
public:
    Game(int w = 20, int h = 12);
    void Init();
    GameField& GetField() noexcept { return field_; }
    const GameField& GetField() const noexcept { return field_; }

    Player& GetPlayer() noexcept { return *player_; }
    const Player& GetPlayer() const noexcept { return *player_; }

    void SetPlayer(std::shared_ptr<Player> p) {
        player_ = std::move(p);
    }
    void ResetField(int w, int h) {
        field_ = GameField(w, h);
        running_ = true;
        state_ = GameState::Running;
    }
    
    bool IsRunning() const noexcept { return running_; }
    void Stop() noexcept { running_ = false; }

    // действия
    void MovePlayer(int dx, int dy);
    void PlayerAttack();
    void PlayerSwitchMode();
    void PlayerCastSpell();
    void UpdateTurn();
    GameState State() const noexcept { return state_; }
    void AdvanceState(); 
    void StartNextLevel();

private:
    bool running_ = true;
    GameState state_ = GameState::Running;
};