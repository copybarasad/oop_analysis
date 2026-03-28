#ifndef INCLUDE_PLAYER
#define INCLUDE_PLAYER

#include <string>
#include "entity.h"
#include "../ui/direction.h"
#include "../items/weapon.h"
#include "../items/inventory.h"
#include "hand.h"

class SpellEnhancement {
private:
    int damage_boost_;
    int area_boost_;
    int summon_boost_;

public:
    SpellEnhancement() : damage_boost_(0), area_boost_(0), summon_boost_(0) {}
    
    void add_boost() {
        damage_boost_ += 10;
        area_boost_ += 1;
        summon_boost_ += 1;
    }
    
    void reset() {
        damage_boost_ = 0;
        area_boost_ = 0;
        summon_boost_ = 0;
    }
    
    bool has_boost() const {
        return damage_boost_ > 0 || area_boost_ > 0 || summon_boost_ > 0;
    }
    
    int damage_boost() const { return damage_boost_; }
    int area_boost() const { return area_boost_; }
    int summon_boost() const { return summon_boost_; }
};


class Player : public Entity {
private:
    std::string name_;
    int score_;
    Weapon weapon_;
    Inventory inventory_;
    Hand hand_;

    bool has_trap_;
    SpellEnhancement enhancement_;

    int max_health_bonus_;
    int weapon_damage_bonus_;
    int spell_power_bonus_;
    int max_inventory_bonus_;
public:
    Player();
    Player(std::string name, int health);
    
    const std::string name() const;
    int damage() const;
    int score() const;

    void add_score(int points);
    Position NextPosition(Direction dir) const;

    const Weapon &weapon() const;
    void change_weapon();

    const Inventory& inventory() const;
    Inventory& inventory();

    const Hand& hand() const;
    Hand& hand();

    void heal(int amount);
    bool spend_score(int amount);


    bool has_trap() const;
    void set_trap(bool has_trap);

    SpellEnhancement& enhancement() { return enhancement_; }
    const SpellEnhancement& enhancement() const { return enhancement_; }

    int max_health_bonus() const { return max_health_bonus_; }
    int weapon_damage_bonus() const { return weapon_damage_bonus_; }
    int spell_power_bonus() const { return spell_power_bonus_; }
    int max_inventory_bonus() const { return max_inventory_bonus_; }
    
    void upgrade_max_health(int bonus) { max_health_bonus_ += bonus; }
    void upgrade_weapon_damage(int bonus) { weapon_damage_bonus_ += bonus; }
    void upgrade_spell_power(int bonus) { spell_power_bonus_ += bonus; }
    void upgrade_max_inventory(int bonus) { max_inventory_bonus_ += bonus; }
    
    int total_max_health() const { return max_health() + max_health_bonus_; }
    int total_weapon_damage() const { return weapon_.damage() + weapon_damage_bonus_; }
    int total_spell_power() const { return spell_power_bonus_; }
};

#endif