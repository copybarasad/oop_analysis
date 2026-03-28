#ifndef ENHANCEMENTSPELL_HPP
#define ENHANCEMENTSPELL_HPP

#include "spell.hpp"
#include "player.hpp"
#include <iostream>

class EnhancementSpell : public Spell {
private:
    int power_multiplier_ = 1;
public:
    bool UseSpell(Game& game) override;
    std::string GetName() const noexcept override { return "Усиление"; }

    static int ApplyRadiusBonus(Player& player, int baseRadius);
    static int ApplyAreaBonus(Player& player, int baseArea);
    static int ApplyDamageBonus(Player& player, int baseDamage, int bonus = 5);
    static int ApplySummonBonus(Player& player, int baseCount);
    void Upgrade(); 
};

#endif