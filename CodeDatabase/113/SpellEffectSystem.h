#ifndef SPELLEFFECTSYSTEM_H
#define SPELLEFFECTSYSTEM_H

#include <string>

class Player;
class GameObject;

class SpellEffectSystem {
public:
    static void applyDamageWithReward(Player& caster, GameObject& target, int damage, const std::string& spellName, const std::string& targetType = "target");
    static void applyDamageToTarget(GameObject& target, int damage, const std::string& spellName, const std::string& targetType = "target");
    static void grantKillReward(Player& caster, const std::string& targetType = "enemy");
};

#endif // SPELLEFFECTSYSTEM_H