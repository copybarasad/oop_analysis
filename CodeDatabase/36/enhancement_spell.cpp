#include "enhancement_spell.h"
#include "player.h"
#include <iostream>

EnhancementSpell::EnhancementSpell() : stacks_(1) {}

bool EnhancementSpell::Cast(Field& /*field*/, Player& player, const std::pair<int,int>& /*target*/,
                           std::vector<Enemy>& /*enemies*/, std::vector<EnemyTower>& /*towers*/, std::vector<EnemyBase>& /*bases*/) {
    for (int i = 0; i < stacks_; ++i) {
        player.AddEnhancementStack();
    }
    std::cout << "Enhancement spell cast: added " << stacks_ << " enhancement stack(s) to player!\n";
    std::cout << "Player now has " << player.GetEnhancementStacks() << " total enhancement stack(s).\n";
    return true;
}

std::string EnhancementSpell::Name() const { return "Enhancement"; }
std::string EnhancementSpell::Description() const {
    return "Adds " + std::to_string(stacks_) + " enhancement stack(s) to empower next spell(s)";
}

void EnhancementSpell::Enhance() { ++stacks_; }
bool EnhancementSpell::CanBeEnhanced() const { return true; }
