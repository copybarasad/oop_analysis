#pragma once

class IDamageSpell;
class ISpawnSpell;
class IUpgradeSpell;

class ISpellContext{
public:
    virtual ~ISpellContext() = default;
    virtual void visit(IDamageSpell& spell) = 0;
    virtual void visit(ISpawnSpell& spell) = 0;
    virtual void visit(IUpgradeSpell& spell) = 0;
};