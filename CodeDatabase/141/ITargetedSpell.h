#ifndef ITARGETED_SPELL_H
#define ITARGETED_SPELL_H

class ISpellContext;

class ITargetedSpell {
public:
    virtual ~ITargetedSpell() = default;

    virtual bool castAt(ISpellContext &context, int targetX, int targetY) = 0;

    virtual int getRange() const = 0;
};

#endif
