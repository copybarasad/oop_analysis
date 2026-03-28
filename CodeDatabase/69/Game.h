#pragma once
#include "Player.h"
#include "Tower.h"
#include "Enemy.h"
#include "Ally.h"
#include "Trap.h"
#include "Hand.h"
#include <vector>
#include <memory>
#include <unordered_map>
#include <string>
#include <iostream>


class Entity;
class Tower;
class Player;
class Enemy;
class Ally;
class Trap;
class Hand;


class Game
{
public:
    Game(int width, int height);
    void CleanupDeadAndRewardPlayer(Player& player);
    void Render(Player const& player) const;
    void Run();
    void ProcessTurn(); // New method for GameController

    //************************************************
    // Сохранение/загрузка
    void Save(const std::string& filename) const;
    void Load(const std::string& filename);

    // Новый старт уровня или новая игра
    void StartNew(int level = 1); // подготовить игру для уровня
    // void StartGameFromMenu();     // Moved to GameController
    // void ShowMainMenu();          // Moved to ConsoleRenderer/GameView
    // std::string ShowLoadFilesMenu(); // Moved to ConsoleRenderer/GameController
    // void ContinueGame();          // Moved to GameController

    int GetLevel() const { return level_; }
    void SetLevel(int l) { level_ = l; }

    // For upgrade menu
    void ApplyUpgrade(int choice);

    const std::vector<std::unique_ptr<Enemy>>& GetEnemies() const { return enemies_; }

    // Getters for Renderer
    int GetWidth() const { return width_; }
    int GetHeight() const { return height_; }
    const std::vector<std::unique_ptr<Tower>>& GetTowers() const { return towers_; }
    const std::vector<std::unique_ptr<Ally>>& GetAllies() const { return allies_; }
    const std::vector<std::unique_ptr<Trap>>& GetTraps() const { return traps_; }
    
    // Game Loop Control
    bool IsGameOver() const { return gameOver_; }
    void SetGameOver(bool state) { gameOver_ = state; }

private:
    int width_;
    int height_;
    Position playerPos_;
    std::vector<std::unique_ptr<Tower>> towers_;
    std::vector<std::unique_ptr<Enemy>> enemies_;
    std::vector<std::unique_ptr<Ally>> allies_;
    std::vector<std::unique_ptr<Trap>> traps_;
    int playerCoinsForKills_ = 0;
    int turnCount_ = 0; // Declare turnCount_ here

    bool gameOver_;
    std::unique_ptr<Player> player_{nullptr};

    //******************************
    int level_ = 1;

public:
    void SetPlayerPosition(Position pos) { playerPos_ = pos; }
    Position GetPlayerPosition() const; // Add this back
    bool IsInBounds(Position p) const;
    bool IsOccupied(Position p) const;
    bool IsInRange(Position from, Position to, int range) const;
    bool DamageEnemyOrTowerAt(Position target, int damage);
    void DamageArea(Position topLeft, int sizeX, int sizeY, int damage);
    int  FetchAndClearCoinPool();
    void AddEnemy(std::unique_ptr<Enemy> enemy);
    void AddTower(std::unique_ptr<Tower> tower);
    void MoveEnemies();
    void CheckTraps();
    void AlliesTurn();
    void TowerAttackPlayer(Tower& tower);
    void SpawnAlly(Position pos);
    bool PlaceTrap(Position pos, const std::string& name, int damage);
    bool DamagePlayerAt(Position pos, int dmg);
    const Player* GetPlayer() const; // Make GetPlayer() const
    Player*  GetPlayer(); // Overload for non-const access when needed

};