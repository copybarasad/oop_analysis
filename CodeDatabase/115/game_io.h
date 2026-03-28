#ifndef GAME_IO_H
#define GAME_IO_H

#include "player.h"
#include "enemy.h"
#include "playing_field.h"
#include "entity_manager.h"
#include "enemy_tower.h"
#include <vector>
#include <string>

class GameIO {
public:
    static void displayGameInfo(const Player& player, const std::vector<Enemy>& enemies, const EntityManager& entity_manager, const std::vector<EnemyTower>& towers);
    static void displayField(const Playing_field& field, const Player& player, const std::vector<Enemy>& enemies, const EntityManager& entity_manager,const std::vector<EnemyTower>& towers);
    static void displayMessage(const std::string& message);

    static void displayControls();
    
    static void displaySpellInfo(const Player& player);
};

#endif