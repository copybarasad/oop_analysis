#ifndef SUMMON_SPELL_H
#define SUMMON_SPELL_H

#include "ISpell.h"

class SummonSpell : public ISpell {
public:
    SummonSpell(int level = 1, int ally_count = 1, int range = 2, int mana_cost = 40)
        : ISpell(SpellType::SUMMON, range, mana_cost, level),
        ally_count_(ally_count) {
            reset_to_base();
        }
    
    bool cast_by_player(GameField& field, Player& player, int target_x = -1, int target_y = -1) override;
    bool needs_target() const override { return false; }
    std::string get_name() const override { return "SummonAlly"; }
    std::string get_description() const override { 
        return "Summons " + std::to_string(ally_count_) + " ally(ies) to fight for you"; 
    }
    std::string get_upgrade_info() const override {
        return "Ally count +1, Mana cost -5";
    }

    int get_ally_count() const { return ally_count_; }
    void reset_to_base() override {
        ally_count_ = level_;
        range_ = 2;
        mana_cost_ = 40 - (level_ - 1) * 5;
    }
    bool upgrade() override;
    
    void set_ally_count(int count) { ally_count_ = count; }

private:
    int ally_count_;
};

#endif