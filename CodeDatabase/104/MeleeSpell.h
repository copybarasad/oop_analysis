#ifndef MELEESPELL_H
#define MELEESPELL_H

#include "SpellCard.h"

// «аклинание ближнего бо€ - наносит урон вокруг игрока
class MeleeSpell : public SpellCard {
private:
    int damage_;
    int mana_cost_;
    bool isRangeEnhanced_;

public:
    MeleeSpell(int damage, int mana_cost); // ”бираем range из параметров!

    bool canCast(const Player& caster, const GameField& field,
        const SpellCastInfo& cast_info) const override;
    bool cast(Player& caster, GameField& field,
        const SpellCastInfo& cast_info) override;
    std::unique_ptr<SpellCard> clone() const override;

    int getManaCost() const override { return mana_cost_; }
    int getCastRange() const override { return 0; } 
    int getDamage() const override { return damage_; }
    bool isAreaSpell() const override { return true; }

    void increaseDamage(int amount) override { damage_ += amount; }
    void increaseRange(int amount) override {
        isRangeEnhanced_ = true;
    }
};

#endif