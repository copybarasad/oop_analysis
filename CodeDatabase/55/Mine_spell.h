#pragma once
#include "Spell.h"
#include "Enemy.h"
#include "Octopus.h"
#include <vector>
#include <algorithm>


class Mine_spell : public Spell{
private:
    GameConfig config;
    int damage;
    int range;
    int cost;
    SpellFlag flag;

public:
    Mine_spell(GameConfig &cfg);
    ~Mine_spell() override = default;
     int getDamage() override;
    void setDamage(int amount);
    int getRange() override;
    void setRange(int amount);
    int getCost() override;
    void setCost(int amount);
    SpellFlag getSpellType() override;
    std::string getSpellTypeString() const override;
    bool attack(Player& pl, Sea& sea, std::vector<Enemy>& enemies, std::vector<Octopus>& octopuses, const Position& center) override;

    void saveTo(SaveFileRAII &out) const override;
    void loadFrom(SaveFileRAII& in) override;
};