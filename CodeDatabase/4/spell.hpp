#pragma once
#include <string>

class Field;
class EntityManager;
class Hero;

class Spell {
public:
    virtual ~Spell() = default;
    virtual bool cast(Field& field, EntityManager& roster, Hero& player, int tx, int ty) = 0;
    virtual std::string name() const = 0;
    virtual int cost() const = 0;
    virtual void upgrade() {}
    virtual int get_spell_id() const = 0;
    virtual int get_upgrade_count() const { return 0; }
    virtual void load_state(std::istream& in) { (void)in; } // Добавить (void)in
};