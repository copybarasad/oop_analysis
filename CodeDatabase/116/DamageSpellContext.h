#include "ISpellContext.h"
#include "DamageSpellResults.h"
#include "IDamageSpell.h" 
#include "../world/GameWorld.h"

class IDamageSpell;
class ISpawnSpell;
class IUpgradeSpell;

class DamageSpellContext : public ISpellContext{
GameWorld& world;
std::pair<int, int> target;
damageSpellResults results;
public:
    DamageSpellContext(GameWorld& world, std::pair<int, int> target) : world(world), target(target), results() {};
    ~DamageSpellContext() = default;
    void visit(IDamageSpell& spell) override;
    void visit(ISpawnSpell& spell) override;
    void visit(IUpgradeSpell& spell) override;
    Character* getTarget(std::pair<int,int> target);
    damageSpellResults getResults() {return results; }
    bool isTargetInRadius(int R);
    std::pair<int, int> getTargetCords() {return target;}
};