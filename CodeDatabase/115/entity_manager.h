#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include "player.h"
#include "enemy.h"
#include "ally.h"
#include "playing_field.h"
#include "trap_spell.h" 
#include <vector>
#include <memory>

class EntityManager {
private:
    Playing_field& field;
    Player& player;
    std::vector<Enemy>& enemies;
    std::vector<Ally> allies;
    std::vector<std::unique_ptr<TrapSpell>> active_trap_spells;  

public:
    EntityManager(Playing_field& field, Player& player, std::vector<Enemy>& enemies);
    
    void spawnEnemies(int count);
    
    void add_ally(const Ally& ally);
    void update_allies(const Playing_field& field, std::vector<Enemy>& enemies);
    void clear_allies();
    int get_ally_count() const;
    const std::vector<Ally>& get_allies() const;
    
    void add_trap_spell(std::unique_ptr<TrapSpell> trap_spell);
    void check_traps();
    std::vector<std::pair<int, int>> get_active_trap_positions() const;
    int get_trap_count() const;
    void update_traps();
    
    const Player& getPlayer() const;
    const std::vector<Enemy>& getEnemies() const;
};

#endif