#include "entity_manager.hpp"
#include "game_objects/player.hpp"
#include "game_objects/enemy.hpp"
#include "game_objects/tower.hpp"
#include "game_objects/spells/damage_spell.hpp"
#include "game_objects/spells/aoe_spell.hpp"
#include "game_objects/spells/trap_spell.hpp"
#include <iostream>

eManager::eManager() 
    : player(nullptr), 
      playerHand(nullptr), 
      tower(nullptr),
      map(nullptr) {
}

eManager::~eManager() {
    if (player) {
        delete player;
    }
    if (playerHand) {
        delete playerHand;
    }
    if (tower) {
        delete tower;
    }
    
    for (Entity* enemy : enemies) {
        if (enemy) {
            delete enemy;
        }
    }
    enemies.clear();

    if (map) {
        delete map;
    }
}

Entity* eManager::getPlayer() {
    return player;
}

SpellHand* eManager::getPlayerHand() {
    return playerHand;
}

std::vector<Entity*>& eManager::getEnemies() {
    return enemies;
}

Entity* eManager::getTower() {
    return tower;
}

std::vector<Entity*> eManager::getEntities() {
    std::vector<Entity*> entities;
    if (player->isAlive()) entities.push_back(player);

    for (size_t i = 0; i < enemies.size(); i++) {
        if(enemies[i]->isAlive())
            entities.push_back(enemies[i]);
    }

    if (tower->isAlive()) entities.push_back(tower);

    return entities;
}

Map* eManager::getMap() {
    return map;
}

const Entity* eManager::getPlayer() const { return player; }

const SpellHand* eManager::getPlayerHand() const { return playerHand; }

const std::vector<Entity*> eManager::getEnemies() const { return enemies; }

const Entity* eManager::getTower() const { return tower; }

const std::vector<Entity*> eManager::getEntities() const { 
    std::vector<Entity*> entities;
    
    if (player) 
        entities.push_back(player);

    entities.insert(entities.end(), enemies.begin(), enemies.end());

    if (tower) 
        entities.push_back(tower);

    return entities;
}

const Map* eManager::getMap() const { return map; }

void eManager::GiveRandomSpellToPlayer() {
    if (!playerHand) return;
    if (playerHand->getSize() >= playerHand->getMaxSize()) return;

    std::vector<SpellCard*> allSpells = {
        new DirectDamageSpell(3, 50),
        new AoeDamageSpell(3, 30),
        new TrapSpell(2, 20)
    };

    std::uniform_int_distribution<> dist(0, allSpells.size() - 1);
    auto newSpell = allSpells[dist(gen)];

    playerHand->addSpell(newSpell);
    std::cout << "Игрок получает новое заклинание: " << newSpell->getName() << "!\n";
}

std::vector<Position> eManager::getPositions() {
    std::vector<Position> positions;

    if (player->isAlive()) {
        positions.push_back(player->getPos());
    }   

    for (size_t i = 0; i < enemies.size(); i++) {
        if(enemies[i]->isAlive())
            positions.push_back(enemies[i]->getPos());
    }

    if (tower->isAlive()) {
        positions.push_back(tower->getPos());
    }

    return positions;
}

void eManager::setPlayer(Entity* newPlayer) {
    if (player) {
        delete player;
    }
    player = newPlayer;
}

void eManager::setPlayerHand(SpellHand* newHand) {
    if (playerHand) {
        delete playerHand;
    }
    playerHand = newHand;
}

void eManager::setTower(Entity* newTower) {
    if (tower) {
        delete tower; 
    }
    tower = newTower;
}

void eManager::clearEnemies() {
    for (Entity* enemy : enemies) {
        if (enemy) {
            delete enemy;
        }
    }
    enemies.clear();
}

void eManager::setEnemy(Entity* enemy) {
    if (enemy) {
        enemies.push_back(enemy);
    }
}

void eManager::setMap(Map* newMap) {
    if (map) {
        delete map;
    }
    map = newMap;
}