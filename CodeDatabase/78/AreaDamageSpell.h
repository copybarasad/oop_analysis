#ifndef AREA_DAMAGE_SPELL_H
#define AREA_DAMAGE_SPELL_H

#include "ISpell.h"

class AreaDamageSpell : public ISpell {
public:
    AreaDamageSpell(int level = 1, int damage = 20, int range = 4, int area_size = 2, int mana_cost = 30)
        : ISpell(SpellType::AREA_DAMAGE, range, mana_cost, level),
        damage_(damage), area_size_(area_size) {
            reset_to_base();
        }
    
    bool cast_by_player(GameField& field, Player& player, int target_x = -1, int target_y = -1) override;
    bool needs_target() const override { return true; }
    std::string get_name() const override { return "AreaDamage"; }
    std::string get_description() const override { 
        return "Deals " + std::to_string(damage_) + " damage in " + std::to_string(area_size_) + "x" + std::to_string(area_size_) + " area"; 
    }
    std::string get_upgrade_info() const override {
        return "Damage +20, Mana cost -5";
    }
    
    int get_damage() const { return damage_; }
    int get_area_size() const { return area_size_; }
    void reset_to_base() override {
        damage_ = 20 * level_;
        range_ = 4;
        area_size_ = 2;
        mana_cost_ = 30 - (level_ - 1) * 5;
    }
    bool upgrade() override;
    
    void set_damage(int damage) { damage_ = damage; }
    void set_area_size(int size) { area_size_ = size; }

private:
    int damage_;
    int area_size_;
};

#endif