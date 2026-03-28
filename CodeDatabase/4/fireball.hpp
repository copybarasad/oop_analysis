#pragma once
#include "spell.hpp"
#include <string>
#include <iosfwd>

class Field;
class EntityManager;
class Hero;

class Fireball : public Spell {
    int base_damage;
    bool is_upgraded = false;
    int upgrade_count = 0;
public:
    int radius;
    Fireball(int dmg = 100, int rad = 9);
    void upgrade() override;
    bool cast(Field& field, EntityManager& roster, Hero& player, int tx, int ty) override;
    std::string name() const override;
    int cost() const override;
    int get_spell_id() const override { return 0; }
    int get_upgrade_count() const override { return upgrade_count; }
    void load_state(std::istream& in) override;
};