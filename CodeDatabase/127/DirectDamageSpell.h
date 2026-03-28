// DirectDamageSpell.h
#ifndef DIRECTDAMAGESPELL_H
#define DIRECTDAMAGESPELL_H
#include "Spell.h"

class Game;
class Player;
class DirectDamageSpell : public Spell {
public:
    DirectDamageSpell(int baseDamage = 30, int range = 3, int price = 10);
    int cost() const override;
    bool apply(Game& game, Player& player, int tx, int ty, const SpellEnhancement& enh, std::string* errorMessage = nullptr) override;
    const char* name() const override { return "DirectDamage"; }
    std::string getDescription() const override;
private:
    int baseDamage_;
    int range_;
    int price_;
};
#endif 