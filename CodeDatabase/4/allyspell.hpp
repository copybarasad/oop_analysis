#pragma once
#include "spell.hpp"

class Field;
class EntityManager;
class Hero;

class AllySpell : public Spell {
    bool is_upgraded = false;
    int summon_count = 1;
    int upgrade_count = 0;
public:
    AllySpell();
    void upgrade() override;
    bool cast(Field& field, EntityManager& roster, Hero& player, int tx, int ty) override;
    std::string name() const override;
    int cost() const override;
    int get_spell_id() const override { return 3; }
    int get_upgrade_count() const override { return upgrade_count; }
    void load_state(std::istream& in) override;
};