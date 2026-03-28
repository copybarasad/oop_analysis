#ifndef PLAYER_H
#define PLAYER_H

#include <memory>
#include <string>
#include "GameObject.h"
#include "../field/Position.h"
#include "../enums/Enums.h"
#include "../spells/SpellHand.h"

class Player : public GameObject {
    Position position;
    int max_health;
    int health;
    int melee_damage;
    int ranged_damage;
    int ranged_range;
    int score;
    AttackType current_attack;
    bool is_switch_weapon;
    SpellHand spellHand;
    int enemies_defeated;
    int level;
    
public:
    Player(int startX, int startY, int handSize = 3, int lvl = 1);
    
    bool isAlive() const override;
    void takeDamage(int mount_damage) override;
    Position getPosition() const override;
    
    void heal(int amount_heal);
    void addScore(int points);
    void addEnemyDefeated();
    void moving(const Position& new_pos);
    void switchWeapon();
    bool buySpell();
    void prepareForNextLevel();
    
    bool getIsSwitchingWeapon() const;
    AttackType getAtackType() const;
    int getDamage() const;
    int getRangedRange() const;
    std::string getWeaponName() const;
    int getHealth() const;
    int getMaxHealth() const;
    int getScore() const;
    int getLevel() const;
    SpellHand& getSpellHand();
    
    std::string serialize() const override;
    static std::shared_ptr<Player> deserialize(const std::string& data);
};

#endif