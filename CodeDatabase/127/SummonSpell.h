// SummonSpell.h
#ifndef SUMMONSPELL_H
#define SUMMONSPELL_H
#include "Spell.h"
#include <string> 

class Game;
class Player;
class SummonSpell : public Spell {
public:
    SummonSpell(int summons = 1, int price = 10);
    SummonSpell(const SummonSpell&) = default;
    SummonSpell(SummonSpell&&) noexcept = default;
    SummonSpell& operator=(const SummonSpell&) = default;
    SummonSpell& operator=(SummonSpell&&) noexcept = default;
    int cost() const override;
    bool apply(Game& game, Player& player, int tx, int ty, const SpellEnhancement& enh, std::string* errorMessage = nullptr) override;
    const char* name() const override { return "Summon"; }
    std::string getDescription() const override;
private:
    int summons_;
    int price_;
};
#endif 