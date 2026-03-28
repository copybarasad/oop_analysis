#ifndef BUFFSPELL_H
#define BUFFSPELL_H

#include "Spell.h"
#include <string>

class Game;
class Player;

class BuffSpell : public Spell {
public:
    explicit BuffSpell(const SpellEnhancement& enh = SpellEnhancement(), int price = 6);

    BuffSpell(const BuffSpell&) = default;
    BuffSpell(BuffSpell&&) noexcept = default;
    BuffSpell& operator=(const BuffSpell&) = default;
    BuffSpell& operator=(BuffSpell&&) noexcept = default;

    int cost() const override;
    bool apply(Game& game, Player& player, int tx, int ty, const SpellEnhancement& enhUnused, std::string* errorMessage = nullptr) override;

    const char* name() const override { return "Buff"; }
    std::string getDescription() const override;

private:
    SpellEnhancement enhancement_;
    int price_;
};

#endif 