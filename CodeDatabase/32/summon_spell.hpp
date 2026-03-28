#ifndef SUMMON_SPELL_HPP
#define SUMMON_SPELL_HPP

#include "spell.hpp"
#include <string>

class SummonSpell : public Spell {
private:
    int health_;
    int damage_;
public:
    SummonSpell(int health = 40, int damage = 8); 
    std::string GetName() const noexcept override;
    bool UseSpell(Game& game) override;
    void Upgrade(); 
};

#endif