#ifndef TRAPSPELL_H
#define TRAPSPELL_H

#include "SpellCard.h"

// Заклинание ловушки - создает ловушку на поле
class TrapSpell : public SpellCard {
private:
    int damage_;
    int mana_cost_;
    bool isAreaEnhanced_;

public:
    TrapSpell(int damage, int mana_cost); // Только 2 параметра!

    bool canCast(const Player& caster, const GameField& field,
        const SpellCastInfo& cast_info) const override;
    bool cast(Player& caster, GameField& field,
        const SpellCastInfo& cast_info) override;
    std::unique_ptr<SpellCard> clone() const override;

    int getManaCost() const override { return mana_cost_; }
    int getCastRange() const override { return 0; } // Ловушка не имеет радиуса каста
    int getDamage() const override { return damage_; }
    bool isAreaSpell() const override { return isAreaEnhanced_; }

    void increaseDamage(int amount) override { damage_ += amount; }
    void increaseRange(int amount) override {
        // Для ловушки увеличение радиуса превращает ее в мину
        isAreaEnhanced_ = true;
    }
};

#endif