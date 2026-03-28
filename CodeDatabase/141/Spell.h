#ifndef SPELL_H
#define SPELL_H

#include <string>
#include <iosfwd>

enum class SpellType {
    DIRECT_DAMAGE,
    AREA_DAMAGE,
    TRAP,
    SUMMON,
    ENHANCE
};

class ISpellContext;
struct Enhancement;

class Spell {
public:
    virtual ~Spell() = default;

    virtual bool cast(ISpellContext &context) = 0;

    virtual bool castAt(ISpellContext &context, int targetX, int targetY) = 0;

    virtual std::string getName() const = 0;

    virtual std::string getDescription() const = 0;

    virtual SpellType getType() const = 0;

    virtual int getRange() const { return 0; }

    virtual bool enhance(const Enhancement &enhancement) = 0;

    virtual Spell *clone() const = 0;

    virtual Spell *upgradedCopy() const = 0;

    virtual bool requiresTarget() const = 0;

    virtual void serialize(std::ostream &out) const = 0;
};

#endif
