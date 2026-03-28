#ifndef GAME_H
#define GAME_H

#include "FightManager.h"
#include "MoveManager.h"
#include "gamestate.h"
#include "StoreGame.h"
#include "player.h"
#include "enemy.h"
#include "field.h"
#include "hand.h"
#include "GameTypes.h"
#include <vector>
#include <string>

class Game {
private:
    Field field;
    Player player;
    std::vector<Enemy> enemies;
    int playerX;
    int playerY;
    std::vector<std::pair<int, int>> enemyPositions;

    Hand hand;
    int lastSpellCount = 0;

    bool isLevelActive;
    int currentLevel = 0;
    double chance;
    int baseEnemyCount;
    bool justLoaded = false;
    
    std::string lastAction;

public:
    Game(int width, int height, double chance, int countenemy);
    ~Game() = default;

    // тут все геттеры увы оч нужны
    const Field& GetField() const { return field; }
    Field& GetField() { return field; }
    const Player& GetPlayer() const { return player; }
    Player& GetPlayer() { return player; }
    int GetPlayerX() const { return playerX; }
    int GetPlayerY() const { return playerY; }
    const std::vector<Enemy>& GetEnemies() const { return enemies; }
    std::vector<Enemy>& GetEnemies() { return enemies; }
    const std::vector<std::pair<int,int>>& GetEnemyPositions() const { return enemyPositions; }
    std::vector<std::pair<int,int>>& GetEnemyPositions() { return enemyPositions; }
    const Hand& GetHand() const { return hand; }
    Hand& GetHand() { return hand; }
    
    std::string GetLastAction() const { return lastAction; }
    void ClearLastAction() { lastAction = ""; }
    std::string GetPlayerInfo() const;
    std::string GetLevelInfo() const;
    
    // всякое
    bool PlayerIsAlive() const { return player.IsLife(); }
    bool IsLevelCompleted() const { return enemies.empty(); }
    bool IsLevelActive() const { return isLevelActive; }
    void SetLevelActive(bool active) { isLevelActive = active; }
    int GetCurrentLevel() const { return currentLevel; }
    std::string GetSpellsInfo() const { return hand.GetSpellsInfo(); }
    

    void GenerateLevel(int width, int height, double chance, int countenemy);
    bool NextLevel();
    
    bool MovePlayerDirection(int dx, int dy);
    AttackResult PerformAttackNearest();
    SpellResult UseSpellByIndex(int index);
    SpellResult UseSpellByIndexWithCoords(int index, int targetX, int targetY);
    bool SwitchPlayerAttackType();
    std::vector<EnemyInfo> SearchForEnemies();
    bool ApplyUpgrade(int choice);
    bool HasSpellRequiringCoords(int index) const;  
    
    std::vector<std::string> EnemyTurn();
    
    GameState SaveToState() const;
    void RestoreFromState(const GameState& state);
    bool SaveGameToFile(const std::string& filename);
    bool LoadGameFromFile(const std::string& filename);

    bool CanMoveTo(int x, int y) const;
    bool UpdateAfterKill();
    void MoveEnemies();
    std::vector<std::string> EnemiesAttackIfPlayerNear();
};

#endif