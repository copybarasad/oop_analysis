#ifndef COMBAT_S_H
#define COMBAT_S_H

#include <chrono>
#include <string>

#include "../../Entities/Player/Player.hpp"
#include "../../Entities/Enemies/Enemy.hpp"

class CombatService {
    public:
    struct CombatResult {
        bool success;
        std::vector<size_t> destroyedEntities; 

        CombatResult () : success(true), destroyedEntities({}) {}
    };

    CombatService () = default;

    CombatResult performCloseFight (Player& player, Entity& enemy);
    CombatResult performRangedAttack (Player& player, Entity& enemy);
    CombatResult performNpcFight (Player& player, Entity& npc, Entity& enemy);
    bool canAttack (const Player& player, unsigned int distance) const;

    private:
    unsigned int calculatePoints (unsigned int enemyHealth);
    void checkEnemy (CombatResult& result, Player& player, Entity& enemy, unsigned int enemyHealth);
    void checkNpc (CombatResult& result, Entity& npc);
};

#endif