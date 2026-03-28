// AreaDamageSpell.h
#ifndef AREADAMAGESPELL_H
#define AREADAMAGESPELL_H
#include "Spell.h"

class AreaDamageSpell : public Spell {
public:
    AreaDamageSpell(int baseDamage = 20, int area = 2, int price = 12);
    int cost() const override;
    bool apply(Game& game, Player& player, int tx, int ty, const SpellEnhancement& enh, std::string* errorMessage = nullptr) override;
    const char* name() const override { return "AreaDamage"; }
    std::string getDescription() const override;
private:
    int baseDamage_;
    int area_;
    int price_;
};
#endif 