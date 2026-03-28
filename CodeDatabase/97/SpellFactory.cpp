#include "SpellFactory.h"
#include "DirectDamageSpell.h"
#include "AreaDamageSpell.h"
#include "TrapSpell.h"
#include "SummonSpell.h"
#include "UpgradeSpell.h"


std::shared_ptr<Spell> SpellFactory::create(const std::string& name,
                                            Field& field,
                                            Coord casterPos,
                                            std::optional<Coord> target,
                                            std::shared_ptr<Player> caster,
                                            bool applyUpgrades,
                                            const UpgradeData& upgrades,
                                            Hand* handPtr) {
    if (name == "DirectDamage") {
        int baseDmg = 4;
        int baseRange = 3;
        int range = baseRange + (applyUpgrades ? upgrades.addRange : 0);
        return std::make_shared<DirectDamageSpell>(baseDmg, range, field, casterPos, target, caster);
    }
    if (name == "AreaDamage") {
        int baseDmg = 3;
        int baseRange = 4;
        int baseW = 2;
        int baseH = 2;
        int w = baseW + (applyUpgrades ? upgrades.addW : 0);
        int h = baseH + (applyUpgrades ? upgrades.addH : 0);
        int range = baseRange + (applyUpgrades ? upgrades.addRange : 0);
        return std::make_shared<AreaDamageSpell>(baseDmg, range, w, h, field, casterPos, target, caster);
    }
    if (name == "Trap") {
        int baseDmg = 9;
        int baseRange = 3;
        int dmg = baseDmg + (applyUpgrades ? upgrades.addTrap : 0);
        return std::make_shared<TrapSpell>(dmg, baseRange, field, casterPos, target, caster);
    }
    if (name == "Summon") {
        int baseCount = 1;
        int baseRange = 1;
        int count = baseCount + (applyUpgrades ? upgrades.addSummon : 0);
        return std::make_shared<SummonSpell>(count, baseRange, field, casterPos, target, caster);
    }
    if (name == "Upgrade") {
        int addRange = 1;
        int addW = 1;
        int addH = 0;
        int addTrap = 0;
        int addSummon = 0;
        return std::make_shared<UpgradeSpell>(addRange, addW, addH, addTrap, addSummon, field, casterPos, target, caster, handPtr);
    }
    return nullptr;
}
