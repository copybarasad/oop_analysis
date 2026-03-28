#pragma once

#include <iostream>
#include <memory>
#include "../world/FieldGenerator.h"
#include "../world/GameWorld.h"
#include "../spells/DamageSpellResults.h"

class GameControl{
private:
    std::unique_ptr<GameWorld> world;
    FieldGenerator spawner;
    static const std::vector<std::pair<int, int>> towerDirections;
    static const std::vector<std::pair<int, int>> enemiesDirections;
    bool playerAttack(int targX, int targY, int damage, std::string prompt = "Вы атаковали");
    bool villainAttack(int targX, int targY, int damage, std::string prompt = "Враг атаковал");
    
public:
    GameControl(int width = 20, int height = 20, int enemies = 4, int slowingCells = 5, int wallsCount = 20, int fortressesCount = 2, int lvl = 0);
    ~GameControl();

    GameWorld& getWorld();
    const GameWorld& getWorld() const;

    FieldGenerator& getGenerator() { return spawner; }
    const FieldGenerator& getGenerator() const { return spawner; }

    void setGeneratorParameters(int enemiesCount, int slowingCellsCount, int wallsCount, int fortressesCount) {
        spawner = FieldGenerator(enemiesCount, slowingCellsCount, wallsCount, fortressesCount);
    }

    void setWorld(std::unique_ptr<GameWorld> newWorld) {
        world = std::move(newWorld);
    }

    void movePlayer(int dx, int dy, bool& tookTurn);
    void longRangeAttack(int dx, int dy);
    void castSpell(int index, bool& tookTurn, std::pair<int, int> target = {0,0});
    void castUpgradeSpell(int index, bool& tookTurn, int UpgradeInd);
    void moveAlly();
    void moveEnemies();
    void towerSpawn();
    void FortressAttack();
    void removeDeadEneties();
    bool gameContinues();
    void printSpellResults(damageSpellResults results);
};