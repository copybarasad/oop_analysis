#pragma once
#include "Spell.h"
#include "Octopus.h"


class Jet_spell : public Spell{
private:
    GameConfig config;
    int damage;
    int range;
    int cost;
    SpellFlag flag;

public:
    Jet_spell(GameConfig &cfg);
    ~Jet_spell() override = default;
    int getDamage() override;
    void setDamage(int amount);
    int getRange() override;
    void setRange(int amount);
    int getCost() override;
    void setCost(int amount);
    SpellFlag getSpellType() override;
    std::string getSpellTypeString() const override;
    bool attack(Player& pl, Sea& sea, std::vector<Enemy>& enemies, std::vector<Octopus>& octopuses, const Position& target) override;

    void saveTo(SaveFileRAII &out) const override;
    void loadFrom(SaveFileRAII& in) override;
};