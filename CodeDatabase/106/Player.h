#ifndef PLAYER_H
#define PLAYER_H

#include "../../spells/PlayerHand.h"
#include "../entity/Entity.h"

class Player : public Entity {
public:
    enum class DamageMode { Melee, Ranged };

private:
    PlayerHand hand;
    int meleeDamage;
    int rangedDamage;
    DamageMode damageMode;
    int score;

    int spell_damage_boost = 0;
    int attack_damage_boost = 0;

public:
    Player(const std::string& playerName, int startX = 0, int startY = 0,
            int health = 100, int mdamage = 10, int rdamage = 5);

    void addScore(int sc);
    void addSpell(CommonSpell spell);

    int getDamage() const { return (damageMode == DamageMode::Melee ? meleeDamage : rangedDamage) + attack_damage_boost; }
    int getScore() const { return score; }
    PlayerHand getHand() const;
    DamageMode getDamageMode() const { return damageMode; };
    int getSpellDamageBoost() const { return spell_damage_boost; }
    int getAttackDamageBoost() const { return attack_damage_boost; }

    void setSpellDamageBoost(int val) { spell_damage_boost = val; }
    void setAttackDamageBoost(int val) { attack_damage_boost = val; }
    void setHP(int hp) { healthPoints = hp; }
    void setMaxHealth(int health) { maxHealthPoints = health; };

    void toggleDamageMode();
};


#endif