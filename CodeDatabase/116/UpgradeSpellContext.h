#pragma once
#include "ISpell.h"
#include "ISpellContext.h"

class IDamageSpell;
class ISpawnSpell;
class IUpgradeSpell;

class UpgradeSpellContext : public ISpellContext{
private:
    ISpell* spellToUpgrade;
public:
    UpgradeSpellContext(ISpell* spell) : spellToUpgrade(spell) {};
    ~UpgradeSpellContext() = default;
    void visit(IDamageSpell& spell) override;
    void visit(ISpawnSpell& spell) override;
    void visit(IUpgradeSpell& spell) override;
    ISpell* getSpell() {return spellToUpgrade; }
};