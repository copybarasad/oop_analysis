// TrapSpell.h
#ifndef TRAPSPELL_H
#define TRAPSPELL_H
#include "Spell.h"
#include <string> 

class Game;
class Player;
class TrapSpell : public Spell {
public:
    TrapSpell(int damage = 20, int price = 10);
    TrapSpell(const TrapSpell&) = default;
    TrapSpell(TrapSpell&&) noexcept = default;
    TrapSpell& operator=(const TrapSpell&) = default;
    TrapSpell& operator=(TrapSpell&&) noexcept = default;
    int cost() const override;
    bool apply(Game& game, Player& player, int tx, int ty, const SpellEnhancement& enh, std::string* errorMessage = nullptr) override;
    const char* name() const override { return "Trap"; }
    std::string getDescription() const override;
private:
    int damage_;
    int price_;
};
#endif 