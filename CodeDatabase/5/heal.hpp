#ifndef HEAL_HPP
#define HEAL_HPP

#include "spell.hpp"

class HealSpell : public Spell {
private:
    int healAmount;
    
public:
    HealSpell();
    void cast_spell(Player& player, int cordX, int cordY, 
                   std::vector<Enemy>& enemies, const GameField& field) override;
    std::string getType() const override;
};

#endif