#include "include/DirectDamageSpell.h"
#include "../gamecore/include/Manager.h"
#include "../entities/include/Entity.h"
#include "../entities/include/Enemy.h"
#include "../entities/include/EnemyBuilding.h"
#include "../entities/include/EnemyTower.h"
#include "../gamecore/include/CombatManager.h"
#include "../gamecore/include/MathUtils.h"

bool DirectDamageSpell::canCast(const Manager& manager, EntityId casterId, const Position& target,
                                const UpgradeState* up) const {
    auto cposOpt = getCasterPosIfValid(manager, casterId);
    if (!cposOpt) return false;
    Position cpos = *cposOpt;

	if (!manager.inBounds(target)) return false;

	int effectiveRange = range_ + (up ? up->stacks : 0);
	if (manhattan(cpos, target) > effectiveRange) return false;
    
    return true;
}

bool DirectDamageSpell::cast(Manager& manager, EntityId casterId, const Position& target,
                             const UpgradeState* up) {
	if (!canCast(manager, casterId, target, up)) return false;
    
    // Всегда тратим заряд
	if (!consumeOne()) return false;

    // Пытаемся найти цель
	auto occ = manager.entities().occupantAt(target);
	if (!occ) return true; // Каст успешен (заряд потрачен), но цель не задета

    // Проверяем, является ли цель допустимым врагом
    const Entity* tgt = manager.entities().entity(*occ);
    if (!tgt) return true;

    const Entity* casterEnt = manager.entities().entity(casterId);
    if (!casterEnt) return true;

    auto casterType = casterEnt->type();
    auto t = tgt->type();
    bool validTarget = false;

    if (casterType == EntityType::Player) {
        validTarget = (t == EntityType::Enemy || t == EntityType::EnemyBuilding || t == EntityType::EnemyTower);
    } else {
        validTarget = (t == EntityType::Player);
    }

    if (validTarget) {
	    manager.combat().damageAndResolve(casterId, *occ, baseDamage_);
    }
    
	return true;
}

void DirectDamageSpell::save(std::ostream& os) const {
    Spell::save(os);
    os << baseDamage_ << " " << range_ << "\n";
}

void DirectDamageSpell::load(std::istream& is) {
    Spell::load(is);
    is >> baseDamage_ >> range_;
}