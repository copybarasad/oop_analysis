#ifndef ENTITY_MANAGER_HPP
#define ENTITY_MANAGER_HPP

#include "game_objects/entity.hpp"
#include "game_world/map.hpp"
#include "game_world/position.hpp"
#include "game_objects/spells/spell_hand.hpp"

#include <vector>
#include <random>

static std::random_device rd;
static std::mt19937 gen(rd());

class eManager {
private:
    Entity* player;
    SpellHand* playerHand;
    std::vector<Entity*> enemies;
    Entity* tower;
    
    Map* map;

public:
    eManager();
    ~eManager();

    Entity* getPlayer();
    SpellHand* getPlayerHand();
    std::vector<Entity*>& getEnemies();
    Entity* getTower();
    std::vector<Entity*> getEntities();
    Map* getMap();

    const Entity* getPlayer() const;
    const SpellHand* getPlayerHand() const;
    const std::vector<Entity*> getEnemies() const;
    const Entity* getTower() const;
    const std::vector<Entity*> getEntities() const;
    const Map* getMap() const;

    std::vector<Position> getPositions();

    void setPlayer(Entity* newPlayer);
    void setPlayerHand(SpellHand* newHand);
    void setTower(Entity* newTower);
    void clearEnemies();
    void setEnemy(Entity* enemy);
    void setMap(Map* newMap);
    
    void GiveRandomSpellToPlayer();

};

#endif