#include "EnhancementSpell.h"
#include "GameField.h"
#include "Player.h"

EnhancementSpell::EnhancementSpell()
    : Spell("Enhancement", 0, 0, SpellType::ENHANCEMENT),
      enhancement_stacks_(0) {}

bool EnhancementSpell::CanCast(int player_x, int player_y, int target_x, int target_y,
                                GameField& field) const {
    return true;
}

void EnhancementSpell::Cast(int player_x, int player_y, int target_x, int target_y,
                             GameField& field, Player& player,
                             std::vector<std::unique_ptr<Enemy>>& enemies,
                             std::vector<std::unique_ptr<EnemyBuilding>>& enemy_buildings,
                             std::vector<std::unique_ptr<EnemyTower>>& enemy_towers,
                             std::vector<std::unique_ptr<Ally>>& allies) {
    enhancement_stacks_++;
}

void EnhancementSpell::Enhance() {
    enhancement_stacks_++;
    level_++;
}

Spell* EnhancementSpell::Clone() const {
    return new EnhancementSpell(*this);
}

int EnhancementSpell::GetEnhancementStacks() const {
    return enhancement_stacks_;
}

void EnhancementSpell::ResetStacks() {
    enhancement_stacks_ = 0;
}
