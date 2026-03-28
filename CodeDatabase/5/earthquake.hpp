#ifndef EARTHQUAKE_SPELL_HPP
#define EARTHQUAKE_SPELL_HPP
#include "spell.hpp"
class EarthquakeSpell : public Spell{
private:
    int damage;
    
public:
    EarthquakeSpell();
    void cast_spell(Player& player, int cordX, int cordY, std::vector<Enemy>& enemies, const GameField& field) override;
    std::string getType() const override;
};

#endif