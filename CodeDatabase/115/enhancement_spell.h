#ifndef ENHANCEMENT_SPELL_H
#define ENHANCEMENT_SPELL_H

#include "base_spell.h"

class EnhancementSpell : public BaseSpell {
private:
    int enhancement_stacks;

public:
    explicit EnhancementSpell(const std::string& name, int cost, int range = 0);
    
    bool cast(Player& player, const Playing_field& field, int target_x, int target_y, std::vector<Enemy>& enemies) override;
    void apply_enhancement(int enhancement_level) override;
    std::unique_ptr<Spell> clone() const override;
    
    int get_enhancement_stacks() const;
    void reset_stacks();
};

#endif