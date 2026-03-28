#ifndef SPELL_H
#define SPELL_H

#include <memory>
#include <string>

class DirectDamageContext;
class AreaDamageContext;
class EnhancementContext;
class SummonContext;
class TrapContext;

class Spell {
public:
    virtual ~Spell() = default;
    virtual const char* getName() const = 0;
    virtual std::unique_ptr<Spell> clone() const = 0;
    virtual void applyEnhancement() {}
    virtual std::string getSerializedData() const = 0;

    virtual bool apply(const DirectDamageContext&) { return false; }
    virtual bool apply(const AreaDamageContext&) { return false; }
    virtual bool apply(const EnhancementContext&) { return false; }
    virtual bool apply(const SummonContext&) { return false; }
    virtual bool apply(const TrapContext&) { return false; }

    virtual bool canUse(const DirectDamageContext&) const { return false; }
    virtual bool canUse(const AreaDamageContext&) const { return false; }
    virtual bool canUse(const EnhancementContext&) const { return false; }
    virtual bool canUse(const SummonContext&) const { return false; }
    virtual bool canUse(const TrapContext&) const { return false; }
};

#endif