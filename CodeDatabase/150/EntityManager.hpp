#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include "GameState.hpp"
#include <vector>

class EntityManager {
public:
    static void updateEnemies(GameState& state);
    static void removeDeadEnemies(GameState& state);
    static Enemy* getEnemyAt(GameState& state, int x, int y);
    static bool damageEnemyAt(GameState& state, int x, int y, int damage);

    static void updateAllies(GameState& state);
    static Ally* getAllyAt(GameState& state, int x, int y);
    static bool damageAllyAt(GameState& state, int x, int y, int damage);

    static void updateBuildings(GameState& state);
    static void removeDestroyedBuildings(GameState& state);
    static Building* getBuildingAt(GameState& state, int x, int y);
    static bool damageBuildingAt(GameState& state, int x, int y, int damage);

    static void updateTraps(GameState& state);

    static void syncFieldWithEntities(GameState& state);
};

#endif