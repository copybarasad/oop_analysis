#ifndef SUMMON_SPELL_H
#define SUMMON_SPELL_H

#include "Spell.h"

class SummonSpell : public Spell {
public:
    SummonSpell(int range = 2, int summon_count = 1);

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

    int GetSummonCount() const;
    int GetAllyHealth() const;
    int GetAllyDamage() const;

private:
    int summon_count_;
    int ally_health_;
    int ally_damage_;
};

#endif // SUMMON_SPELL_H
