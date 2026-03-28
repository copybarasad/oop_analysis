#ifndef TRAP_SPELL_H
#define TRAP_SPELL_H

#include "ISpell.h"

class TrapSpell : public ISpell {
public:
    TrapSpell(int level = 1, int damage = 50, int range = 3, int mana_cost = 15)
        : ISpell(SpellType::TRAP, range, mana_cost, level),
        damage_(damage) {
            reset_to_base();
        }
    
    bool cast_by_player(GameField& field, Player& player, int target_x = -1, int target_y = -1) override;
    bool needs_target() const override { return true; }
    std::string get_name() const override { return "Trap"; }
    std::string get_description() const override { 
        return "Places a trap that deals " + std::to_string(damage_) + " damage when triggered"; 
    }
    std::string get_upgrade_info() const override {
        return "Damage +50, Mana cost -" + std::to_string(level_ * 5);
    }

    int get_damage() const { return damage_; }
    void reset_to_base() override {
        damage_ = 50 * level_;
        range_ = 3;
        mana_cost_ = 15 - (level_ - 1) * 5;
    }
    bool upgrade() override;
    
    void set_damage(int damage) { damage_ = damage; }

private:
    int damage_;
};

#endif