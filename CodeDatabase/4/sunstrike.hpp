#pragma once
#include "spell.hpp"

class Field;
class EntityManager;
class Hero;

class Sunstrike : public Spell {
    int base_damage;
    int area;
    bool is_upgraded = false;
    int upgrade_count = 0;
public:
    Sunstrike(int dmg = 100, int a = 2);
    void upgrade() override;
    bool cast(Field& field, EntityManager& roster, Hero& player, int cx, int cy) override;
    std::string name() const override;
    int cost() const override;
    int get_spell_id() const override { return 1; }
    int get_upgrade_count() const override { return upgrade_count; }
    void load_state(std::istream& in) override;
};