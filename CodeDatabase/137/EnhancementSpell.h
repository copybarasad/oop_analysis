#ifndef ENHANCEMENT_SPELL_H
#define ENHANCEMENT_SPELL_H

#include "Spell.h"

class EnhancementSpell : public Spell {
public:
    EnhancementSpell();

    bool CanCast(int player_x, int player_y, int target_x, int target_y,
                 GameField& field) const override;
    void Cast(int player_x, int player_y, int target_x, int target_y,
              GameField& field, Player& player,
              std::vector<std::unique_ptr<Enemy>>& enemies,
              std::vector<std::unique_ptr<EnemyBuilding>>& enemy_buildings,
              std::vector<std::unique_ptr<EnemyTower>>& enemy_towers,
              std::vector<std::unique_ptr<Ally>>& allies) override;
    void Enhance() override;
    Spell* Clone() const override;

    int GetEnhancementStacks() const;
    void ResetStacks();

private:
    int enhancement_stacks_;
};

#endif // ENHANCEMENT_SPELL_H
