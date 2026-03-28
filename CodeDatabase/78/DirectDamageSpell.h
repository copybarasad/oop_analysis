#ifndef DIRECT_DAMAGE_SPELL_H
#define DIRECT_DAMAGE_SPELL_H

#include "ISpell.h"

class DirectDamageSpell : public ISpell{
public:
    DirectDamageSpell(int level = 1, int damage = 30, int range = 5, int mana_cost = 20)
        : ISpell(SpellType::DIRECT_DAMAGE, range, mana_cost, level),
        damage_(damage) {
            reset_to_base();
        }
    
    bool cast_by_player(GameField& field, Player& player, int target_x = -1, int target_y = -1) override;
    bool cast_by_tower(Player& player, float multiplier);
    bool needs_target() const override { return true; }
    std::string get_name() const override { return "DirectDamage"; }
    std::string get_description() const override { 
        return "Deals " + std::to_string(damage_) + " damage to a single target in " + std::to_string(get_range()) + " range."; 
    }
    std::string get_upgrade_info() const override {
        return "Damage +30, Mana cost -5";
    }
    int get_damage() const { return damage_; }
    void reset_to_base() override {
        damage_ = 30*level_;
        range_ = 5;
        mana_cost_ = 20 - (level_ - 1) * 5;
    }
    bool upgrade() override;
    
    void set_damage(int damage) { damage_ = damage; }
    void set_range(int range) { range_ = range; }

private:
    int damage_;
};

#endif