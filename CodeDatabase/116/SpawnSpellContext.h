#pragma once
#include "../world/GameWorld.h"
#include "../world/FieldGenerator.h"
#include "ISpellContext.h"

class IDamageSpell;
class ISpawnSpell;
class IUpgradeSpell;

class SpawnSpellContext : public ISpellContext{
GameWorld& world;
FieldGenerator& spawner;
std::pair<int, int> target;
public:
    SpawnSpellContext(GameWorld& world, FieldGenerator& spawner, std::pair<int, int> target) : world(world), target(target), spawner(spawner){};
    ~SpawnSpellContext() = default;
    void visit(IDamageSpell& spell) override;
    void visit(ISpawnSpell& spell) override;
    void visit(IUpgradeSpell& spell) override;
    bool isTargetInRadius(int R);
    bool cellIsEmpty(std::pair<int, int> cords) { return world.getField().getCell(cords.first, cords.second).getType() == Cell::CellType::EMPTY; }
    std::pair<int, int> getTargetCords() {return target;}
    FieldGenerator& getSpawner() {return spawner; }
    GameWorld& getWorld() { return world; }
};