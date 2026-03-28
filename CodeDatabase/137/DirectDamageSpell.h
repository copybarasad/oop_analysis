#ifndef DIRECT_DAMAGE_SPELL_H
#define DIRECT_DAMAGE_SPELL_H

#include "Spell.h"

class DirectDamageSpell : public Spell {
public:
    DirectDamageSpell(int damage = 25, int range = 3);

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

    int GetDamage() const;

private:
    int damage_;
};

#endif // DIRECT_DAMAGE_SPELL_H
