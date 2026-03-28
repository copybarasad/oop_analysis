#ifndef AREA_DAMAGE_SPELL_H
#define AREA_DAMAGE_SPELL_H

#include "Spell.h"

class AreaDamageSpell : public Spell {
public:
    AreaDamageSpell(int damage = 15, int range = 4, int area_size = 2);

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
    int GetAreaSize() const;

private:
    int damage_;
    int area_size_;
};

#endif // AREA_DAMAGE_SPELL_H
