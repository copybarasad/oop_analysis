#ifndef ENHANCEMENTSPELL_H
#define ENHANCEMENTSPELL_H

#include "ISpell.h"

class EnhancementSpell : public ISpell {
public:
    EnhancementSpell(int level = 1, int stacks = 0, int mana_cost = 25)
    : ISpell(SpellType::ENHANCEMENT, 0, mana_cost, level),
      stacks_(stacks) {
        reset_to_base();
    }
    
    bool cast_by_player(GameField& field, Player& player, int target_x = -1, int target_y = -1) override;
    bool needs_target() const override { return false; }
    std::string get_name() const override { return "Enhancement"; }
    std::string get_description() const override { 
        return "Enhances your next spell (stacks: " + std::to_string(stacks_) + ")"; 
    }
    std::string get_upgrade_info() const override {
        return "Stacks +1, Mana cost -5";
    }

    void reset_to_base() override {
        stacks_ = -1 + level_;
        mana_cost_ = 25 - (level_ - 1) * 5;
    }
    bool upgrade() override;
    
    void apply_enhancement(ISpell* spell);
    int get_stacks() const { return stacks_; }
    void add_stack() { stacks_++; }
    void reset_stacks() { stacks_ = 0; }

private:
    int stacks_;
};

#endif