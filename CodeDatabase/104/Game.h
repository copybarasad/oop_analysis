#ifndef GAME_H
#define GAME_H

#include "Player.h"
#include "GameField.h"
#include "Enemy.h"
#include "PlayerUpgrade.h"
#include "GameSaveManager.h"
#include "GameRenderer.h"
#include "InputHandler.h"
#include <vector>
#include <memory>
#include <string>

class GameSaveManager;

class Game {
public:
    enum class CommandResult {
        CONTINUE,
        RESTART_WITH_LOAD,
        QUIT
    };

    Game();
    Game(int startLevel);

    // Основной игровой цикл
    void StartGame();
    void ProcessGameLoop();
    bool IsGameRunning() const;
    CommandResult ProcessInput(const std::string& input);

    // Геттеры для сериализации
    int getCurrentLevel() const { return current_level_; }
    bool isGameWon() const { return game_won_; }
    const Player& getPlayer() const { return *player_; }
    const GameField& getGameField() const { return *field_; }

    // Новые методы для системы сохранения
    void setCurrentLevel(int level) {
        current_level_ = level;
        if (player_ && field_) {
            InitializeLevel(level);
        }
    }
    void setPlayer(std::unique_ptr<Player> player) { player_ = std::move(player); }
    void setGameField(std::unique_ptr<GameField> field) { field_ = std::move(field); }

private:
    std::unique_ptr<Player> player_;
    std::unique_ptr<GameField> field_;
    std::unique_ptr<PlayerUpgrade> player_upgrade_;
    std::unique_ptr<GameSaveManager> save_manager_;
    std::unique_ptr<GameRenderer> renderer_;
    std::unique_ptr<InputHandler> input_handler_;
    int current_level_;
    bool game_running_;
    bool game_won_;

    // Инициализация и управление уровнями
    void InitializeLevel(int level);
    void SpawnEnemiesForLevel(int level);
    void SpawnTowersForLevel(int level);
    void CheckLevelCompletion();
    void LevelUpPlayer();
    void renderGameState();
    void ShowLevelUpMenu();

    // Шаги игрового цикла
    CommandResult processPlayerTurn(const std::string& input);
    void processEnemiesTurn();
    void processGameOver();

    // Система прокачки и заклинаний
    void applyLevelUpgrades();
    void removeHalfOfSpells();
    void showSpellReplacementMenu();
    std::vector<std::string> getMissingSpells() const;
    void addSpellByName(const std::string& spellName);

};

#endif