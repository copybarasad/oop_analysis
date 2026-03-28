#ifndef GAME_H
#define GAME_H

#include <cstddef>
#include <memory>
#include <vector>
#include <string>
#include "GameField.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemyBase.h"
#include "EnemyTower.h"
#include "Ally.h"
#include "IGameObserver.h"

class Spell;

class Game {
public:
    Game(std::size_t width, std::size_t height);

    void AddObserver(IGameObserver *observer);

    void ProcessNonPlayerTurn();

    void PublicSwitchPlayerMode();

    void PublicPlayerRangedAttack();

    void PublicPlayerCastSpell();

    void RenderField();

    void RenderStatus();

    bool IsPlayerAlive() const;

    bool IsQuitRequested() const;

    void MovePlayerUp();

    void MovePlayerDown();

    void MovePlayerLeft();

    void MovePlayerRight();

    void RequestQuit();

    std::size_t CountAliveEnemies() const;

    void Run();

    // === Доступ к полю ===
    GameField &GetField();

    const GameField &GetField() const;

    // === Доступ к игроку ===
    Player &GetPlayer();

    const Player &GetPlayer() const;

    // === Доступ к союзникам ===
    std::vector<Ally> &GetAllies() { return allies_; }
    const std::vector<Ally> &GetAllies() const { return allies_; }

    std::vector<Enemy> &GetEnemies() { return enemies_; }
    const std::vector<Enemy> &GetEnemies() const { return enemies_; }

    // === Доступ к базам ===
    std::vector<EnemyBase> &GetBases() { return bases_; }
    const std::vector<EnemyBase> &GetBases() const { return bases_; }

    // === Доступ к башням ===
    std::vector<EnemyTower> &GetTowers() { return towers_; }
    const std::vector<EnemyTower> &GetTowers() const { return towers_; }

    bool GetPlayerPosition(std::size_t &x, std::size_t &y) const;

    bool DealDirectDamage(std::size_t x, std::size_t y, int damage);

    void DealAreaDamage(std::size_t x, std::size_t y,
                        std::size_t width, std::size_t height,
                        int damage);

    std::unique_ptr<Spell> CreateRandomSpell();

    bool FindEnemyPosition(std::size_t enemy_index,
                           std::size_t &x,
                           std::size_t &y) const;

    void CleanupDead();

    void SaveToFile(const std::string &filename) const;

    void LoadFromFile(const std::string &filename);

private:
    enum class Direction {
        kUp,
        kDown,
        kLeft,
        kRight
    };

    // === Сущности ===
    GameField field_;
    Player player_;
    std::vector<Enemy> enemies_;
    std::vector<EnemyBase> bases_;
    std::vector<EnemyTower> towers_;
    std::vector<Ally> allies_;
    std::vector<IGameObserver*> observers_;


    bool player_skip_turn_next_time_;
    bool quit_requested_;
    std::size_t enemies_killed_ = 0;
    int level_ = 1; // текущий уровень
    bool level_completed_ = false; // флаг "уровень пройден"
    int enemies_to_kill_for_level_ = 7; // сколько врагов нужно убить на уровне
    void ShowLevelUpMenu();

    void StartNextLevel(); // переход на следующий уровень
    void RemoveHalfSpellsRandom(); // удалить половину карт из руки
    // === Инициализация ===
    void Initialize();

    void PlacePlayer();

    void PlaceEnemyBases();

    void PlaceEnemyTowers();

    void PlaceInitialEnemies();

    // === Отрисовка ===
    void ClearScreen() const;

    void PrintField() const;

    void PrintStatus() const;

    // === Ходы ===
    void PlayerTurn();

    void PlayerRangedAttack();

    void PlayerCastSpell();

    void AlliesTurn();

    bool FindAllyPosition(std::size_t ally_index,
                          std::size_t &x,
                          std::size_t &y) const;

    void EnemiesTurn();

    void EnemyBasesTurn();

    void EnemyTowersTurn();

    // === Логика ===
    void MovePlayer(Direction direction);

    void SwitchPlayerMode();

    void MoveSingleEnemy(std::size_t enemy_index);


    bool FindBasePosition(std::size_t base_index,
                          std::size_t &x,
                          std::size_t &y) const;

    bool FindTowerPosition(std::size_t tower_index,
                           std::size_t &x,
                           std::size_t &y) const;

    void SpawnEnemyNearBase(std::size_t base_index,
                            std::size_t base_x,
                            std::size_t base_y);

    void ProcessPlayerCell(std::size_t x, std::size_t y);

    void OnEnemyKilled();

    void EnsureInvariants() const;

    bool LevelCompleted() const;

    void ResetLevel();

    void Notify(const GameEvent& event);
};

#endif
