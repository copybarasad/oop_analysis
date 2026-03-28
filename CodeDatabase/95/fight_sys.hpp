#ifndef FIGHT_SYS_HPP
#define FIGHT_SYS_HPP

#include "game_objects/entity.hpp"
class eManager;

class Fighter {
public:
    static void applyDamage(Entity& source, Entity& target, int dmg);

    static void HandAttack(Entity& attacker, eManager& manager);

    static bool CastDirectDamage(Entity& caster, eManager& manager, class DirectDamageSpell& spell);
    static bool CastAoeDamage(Entity& caster, eManager& manager, class AoeDamageSpell& spell, int startX, int startY);
    static bool CastTrap(Entity& caster, eManager& manager, class TrapSpell& spell, int targetX, int targetY);

};

#endif
