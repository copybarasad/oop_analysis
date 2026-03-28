#pragma once
#include <string>


class DirectSpell;
class SquareSpell;
class TrapSpell;
class SummonSpell;
class UpgradeSpell;


class SpellCardVisitor {
public:
    virtual ~SpellCardVisitor() = default;
    virtual std::string format(const DirectSpell* spell) const = 0;
    virtual std::string format(const SquareSpell* spell) const = 0;
    virtual std::string format(const TrapSpell* spell) const = 0;
    virtual std::string format(const SummonSpell* spell) const = 0;
    virtual std::string format(const UpgradeSpell* spell) const = 0;
};


class SimpleTextFormatter : public SpellCardVisitor {
public:
    std::string format(const DirectSpell* spell) const override {
        return "Direct Spell";
    }
    std::string format(const SquareSpell* spell) const override {
        return "Square Spell";
    }
    std::string format(const TrapSpell* spell) const override {
        return "Trap Spell";
    }
    std::string format(const SummonSpell* spell) const override {
        return "Summon Spell";
    }
    std::string format(const UpgradeSpell* spell) const override {
        return "Upgrade Spell";
    }
};