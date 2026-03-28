#pragma once
#include "spell.hpp"

class Field;
class EntityManager;
class Hero;

class UpgradeSpell : public Spell {
public:
    bool cast(Field& field, EntityManager& roster, Hero& player, int tx, int ty) override;
    std::string name() const override;
    int cost() const override;
    void upgrade() override {}
    int get_spell_id() const override { return 4; }
    int get_upgrade_count() const override { return 0; }
    void load_state(std::istream& in) override {}
};