#include "enhancement_spell.h"
#include "player.h"
#include <iostream>

EnhancementSpell::EnhancementSpell(const std::string& name, int cost, int range)
    : BaseSpell(name, "Enhances next spell cast", range, cost, false), enhancement_stacks(1) {}

bool EnhancementSpell::cast(Player& player, const Playing_field& field, int target_x, int target_y, std::vector<Enemy>& enemies) {
    player.add_enhancement(enhancement_stacks);
    std::cout << name << " cast! Next spell will be enhanced with " << enhancement_stacks << " levels." << std::endl;
    return true;
}

void EnhancementSpell::apply_enhancement(int enhancement_level) {
    enhancement_stacks += enhancement_level;
    std::cout << name << " enhanced! Stacks: " << enhancement_stacks << std::endl;
}

std::unique_ptr<Spell> EnhancementSpell::clone() const {
    return std::make_unique<EnhancementSpell>(*this);
}

int EnhancementSpell::get_enhancement_stacks() const {
    return enhancement_stacks;
}

void EnhancementSpell::reset_stacks() {
    enhancement_stacks = 1;
}