#pragma once
#include "spell.h"
#include "player.h"
class TowerDirectSpell : public Spell {
private:
    std::string name;
    int damage;
    int range;
    
public:
    TowerDirectSpell(int dmg, int radius, std::string spellName) : damage(dmg), range(radius), name(spellName) {}
    bool do_spell(int x, int y, Field& field, Player& player, Enemy& enemy, std::string& message) override;
    bool do_spell(int targetX, int targetY, Player& target, std::string& message) override;
    std::string getName() const override;
    int getCost() const override;
    int getRange() const override;
};