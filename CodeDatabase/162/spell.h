#ifndef SPELL_H
#define SPELL_H

#include <string>
#include <memory>
#include <tuple>

class Player;
class Field;

class Spell {
public:
    virtual ~Spell() = default;
    virtual bool Use(Player& player, Field& field, int targetX, int targetY) = 0;
    virtual std::string GetName() const = 0;
    virtual std::string GetType() const = 0;  // НОВЫЙ МЕТОД
};

class DirectDamageSpell : public Spell {
public:
    bool Use(Player& player, Field& field, int targetX, int targetY) override;
    std::string GetName() const override { return "Direct Damage"; }
    std::string GetType() const override { return "DirectDamage"; }  // ИМПЛЕМЕНТАЦИЯ
};

class AreaDamageSpell : public Spell {
public:
    bool Use(Player& player, Field& field, int targetX, int targetY) override;
    std::string GetName() const override { return "Area Damage"; }
    std::string GetType() const override { return "AreaDamage"; }  // ИМПЛЕМЕНТАЦИЯ
};

class TrapSpell : public Spell {
public:
    bool Use(Player& player, Field& field, int targetX, int targetY) override;
    std::string GetName() const override { return "Trap"; }
    std::string GetType() const override { return "Trap"; }  // ИМПЛЕМЕНТАЦИЯ
};

class SummonSpell : public Spell {
public:
    bool Use(Player& player, Field& field, int targetX, int targetY) override;
    std::string GetName() const override { return "Summon"; }
    std::string GetType() const override { return "Summon"; }  // ИМПЛЕМЕНТАЦИЯ
};

class BuffSpell : public Spell {
public:
    bool Use(Player& player, Field& field, int targetX, int targetY) override;
    std::string GetName() const override { return "Buff"; }
    std::string GetType() const override { return "Buff"; }  // ИМПЛЕМЕНТАЦИЯ
};

#endif
