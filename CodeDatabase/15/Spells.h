#ifndef SPELLS_H
#define SPELLS_H

#include "ISpell.h"

class DirectDamageSpell : public ISpell {
public:
    DirectDamageSpell(int damage, int range);
    bool cast(Game& game, Entity& caster, const Point& target) override;
    std::string getName() const override;
    int getCost() const override;
    SpellType getType() const override;
private:
    int base_damage_;
    int base_range_;
};

class AreaDamageSpell : public ISpell {
public:
    AreaDamageSpell(int damage, int area_size);
    bool cast(Game& game, Entity& caster, const Point& target) override;
    std::string getName() const override;
    int getCost() const override;
    SpellType getType() const override;
private:
    int base_damage_;
    int base_area_size_;
};

class TrapSpell : public ISpell {
public:
    TrapSpell(int damage);
    bool cast(Game& game, Entity& caster, const Point& target) override;
    std::string getName() const override;
    int getCost() const override;
    SpellType getType() const override;
private:
    int base_damage_;
};

class SummonSpell : public ISpell {
public:
    bool cast(Game& game, Entity& caster, const Point& target) override;
    std::string getName() const override;
    int getCost() const override;
    SpellType getType() const override;
};

class EnhancementSpell : public ISpell {
public:
    bool cast(Game& game, Entity& caster, const Point& target) override;
    std::string getName() const override;
    int getCost() const override;
    SpellType getType() const override;
};

#endif