#include "ISpellContext.h"
#include "DamageSpellResults.h"
#include "../world/GameWorld.h"

class IDamageSpell;
class ISpawnSpell;
class IUpgradeSpell;

class EnemySpellContext : public ISpellContext{
GameWorld& world;
std::pair<int, int> selfCords;
damageSpellResults results;
public:
    EnemySpellContext(GameWorld& world, std::pair<int, int> selfCords) : world(world), selfCords(selfCords), results() {};
    ~EnemySpellContext() = default;
    void visit(IDamageSpell& spell) override;
    void visit(ISpawnSpell& spell) override;
    void visit(IUpgradeSpell& spell) override;
    Character* getTarget(std::pair<int,int> target);
    damageSpellResults getResults() { return results; }
    std::pair<int, int> getSelfCords() { return selfCords; }
};