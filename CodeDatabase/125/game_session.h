#pragma once
#include "game_field.h"
#include "player.h"
#include "enemy_manager.h"
#include "console_renderer.h"
#include "command_reader.h" 
#include "save_manager.h" 
#include "ui_manager.h"

class GameSession {
public:
    GameSession(int width, int height);
    
    bool LoadGame(int& level);
    void SaveGame(int level);
    
    void StartLevel(int start_type = 0);
    bool ProcessPlayerTurn(char cmd);
    void EnemiesTurn();
    bool IsLevelComplete() const;
    bool IsGameOver() const;
    void RestartGame();
    void HandleVictory();
    
    const GameField& GetGameField() const { return field_; }
    const Player& GetPlayer() const { return player_; }
    const EnemyManager& GetEnemyManager() const { return enemy_manager_; }

    Player& GetPlayer() { return player_; }
    GameField& GetGameField() { return field_; }
    EnemyManager& GetEnemyManager() { return enemy_manager_; }
    
    void SetLevel(int level) { current_level_ = level; }
    int GetLevel() const { return current_level_; }
    
    static constexpr int MAX_LEVELS = 3;

    void ProcessMovement(Position new_pos);
    void ProcessAttack();
    void ProcessSpellCast(int spellIndex);
    void ProcessSwitchMode();
    void ProcessOpenShop();
    void ProcessSaveGame();

private:
    GameField field_;
    Player player_;
    EnemyManager enemy_manager_;
    int current_level_;
};
