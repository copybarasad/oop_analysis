#ifndef COMBAT_MANAGER_H
#define COMBAT_MANAGER_H

#include "game_field.h"
#include "game_enums.h"
#include "player.h"
#include "enemy.h"
#include "enemy_building.h"
#include "tower.h"
#include "spell_card.h"
#include "direct_damage_spell.h"
#include "area_damage_spell.h"
#include "trap_spell.h"
#include <vector>
#include <memory>

class CombatManager {
private:
    GameField& field;
    std::vector<Position> activeTraps;
    
    bool isInMeleeRange(const Position& playerPos, const Position& enemyPos) const;
    bool isInRangedRange(const Position& playerPos, const Position& enemyPos) const;
    bool canAttack(const Position& playerPos, CombatMode playerMode, const Enemy& enemy) const;
    
    void processDirectDamageSpell(Player& player, DirectDamageSpell* spell, const Position& target,
                                std::vector<Enemy>& enemies, std::vector<EnemyBuilding>& buildings);
    void processAreaDamageSpell(Player& player, AreaDamageSpell* spell, const Position& target,
                              std::vector<Enemy>& enemies, std::vector<EnemyBuilding>& buildings);
    void processTrapSpell(Player& player, TrapSpell* spell, const Position& target);

public:
    explicit CombatManager(GameField& field);
    
    // Const геттеры
    const std::vector<Position>& getActiveTraps() const;
    
    // Не-const геттеры
    std::vector<Position>& getActiveTraps() { return activeTraps; }
    
    // Метод для очистки ловушек
    void clearTraps() { activeTraps.clear(); }
    
    void processPlayerAttack(Player& player, std::vector<Enemy>& enemies);
    void processSpellCast(Player& player, int spellIndex, const Position& target, 
                         std::vector<Enemy>& enemies, std::vector<EnemyBuilding>& buildings,
                         std::vector<Tower>& towers);
    void processTowerAttacks(std::vector<Tower>& towers, Player& player);
    void processTrapEffects(std::vector<Enemy>& enemies);
    void checkTrapTrigger(const Position& position, std::vector<Enemy>& enemies, Player& player);
};

#endif