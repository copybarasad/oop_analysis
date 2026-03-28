#ifndef DIRECTDAMAGESPELL_H
#define DIRECTDAMAGESPELL_H

#include "SpellCard.h"

// «аклинание пр€мого урона - наносит урон одной цели
class DirectDamageSpell : public SpellCard {
private:
    int damage_;
    int range_;
    int mana_cost_;
    bool isAreaEnhanced_;

public:
    DirectDamageSpell(int damage, int range, int mana_cost);

    bool canCast(const Player& caster, const GameField& field,
        const SpellCastInfo& cast_info) const override;
    bool cast(Player& caster, GameField& field,
        const SpellCastInfo& cast_info) override;
    std::unique_ptr<SpellCard> clone() const override;

    int getManaCost() const override { return mana_cost_; }
    int getCastRange() const override { return range_; }
    int getDamage() const override { return damage_; }
    bool isAreaSpell() const override { return isAreaEnhanced_; }

    void increaseDamage(int amount) override { damage_ += amount; }
    void increaseRange(int amount) override {
        range_ += amount;
        isAreaEnhanced_ = true;
    }
};

#endif