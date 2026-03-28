#ifndef GAMEFIELD_H
#define GAMEFIELD_H

#include "position.h"
#include "enemytower.h"
#include "player.h"
#include "spell.h"
#include "movingenemy.h"
#include <vector>
#include <memory>

class Trap {
private:
    Position position;
    int damage;
    bool active;
    
public:
    Trap(const Position& position, int damage);
    bool trigger(Player* player);
    const Position& getPosition() const;
    bool isActive() const;
    int getDamage() const;
    void deactivate();
};

class GameField {
private:
    int width;
    int height;
    std::vector<Trap> traps;
    std::vector<std::unique_ptr<EnemyTower>> towers;
    std::vector<MovingEnemy> enemies;
    Player* externalPlayer;
    
public:
    // Constructors
    GameField(int width = 15, int height = 15);
    ~GameField();
    GameField(const GameField& other);
    GameField(GameField&& other);
    GameField& operator=(const GameField& other);
    GameField& operator=(GameField&& other);
    
    // Basic methods
    int getWidth() const;
    int getHeight() const;
    bool isValidPosition(const Position& position) const;
    void placePlayer(Player* player);
    void clearCell(const Position& position);
    int getAliveEnemyCount() const;
    
    // Lab 2 spell & combat methods
    void placeTrap(const Position& position, int damage);
    void addTower(std::unique_ptr<EnemyTower> tower);
    void addEnemy(const MovingEnemy& enemy);
    const std::vector<MovingEnemy>& getEnemies() const;
    bool hasEnemyAt(const Position& pos) const;
    bool hasBuildingAt(const Position& position) const;
    void applyDamageAt(const Position& position, int damage);
    
    // ADD THESE MISSING METHOD DECLARATIONS:
    void applyTowerDamage(Player* player, int damage);  // WAS MISSING
    void updateTowerCooldowns();  // WAS MISSING
    void checkTraps();
    void display(const std::vector<MovingEnemy>& movingEnemies) const;
    //void addEnemy(const MovingEnemy& enemy);
    
    // Player access
    Player* getPlayer() const;
    void movePlayer(const Position& newPosition);
    
    // Tower access
    const std::vector<std::unique_ptr<EnemyTower>>& getTowers() const {
        return towers;
    }

};

#endif