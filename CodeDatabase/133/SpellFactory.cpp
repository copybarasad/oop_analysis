#include "SpellFactory.h"
#include <random>
#include "GameConstants.h"
#include "AreaDamageSpell.h"
#include "DirectDamageSpell.h"
#include "EnhancementSpell.h"
#include "SummonSpell.h"
#include "TrapSpell.h"

namespace {
	constexpr int DIRECT_DAMAGE_BASE = 15;
	constexpr int DIRECT_DAMAGE_RADIUS = 1;
	constexpr int DIRECT_DAMAGE_UPGRADED = 25;
	constexpr int DIRECT_DAMAGE_UPGRADED_RADIUS = 2;

	constexpr int AREA_DAMAGE_BASE = 10;
	constexpr int AREA_DAMAGE_SIZE = 1;
	constexpr int AREA_DAMAGE_UPGRADED = 20;
	constexpr int AREA_DAMAGE_UPGRADED_SIZE = 2;

	constexpr int TRAP_DAMAGE_BASE = GameConstants::DEFAULT_TRAP_DAMAGE;
	constexpr int TRAP_DAMAGE_UPGRADED = GameConstants::DEFAULT_TRAP_DAMAGE + 10;

	constexpr int SUMMON_HEALTH_BASE = GameConstants::DEFAULT_ALLY_HEALTH;
	constexpr int SUMMON_DAMAGE_BASE = GameConstants::DEFAULT_ALLY_DAMAGE;
	constexpr int SUMMON_HEALTH_UPGRADED = GameConstants::DEFAULT_ALLY_HEALTH + 20;
	constexpr int SUMMON_DAMAGE_UPGRADED = GameConstants::DEFAULT_ALLY_DAMAGE + 3;
}

std::unique_ptr<Spell> SpellFactory::create(SpellId id) {
	switch (id) {
		case SpellId::DIRECT_DAMAGE:
			return std::make_unique<DirectDamageSpell>(DIRECT_DAMAGE_BASE, DIRECT_DAMAGE_RADIUS);
		case SpellId::AREA_DAMAGE:
			return std::make_unique<AreaDamageSpell>(AREA_DAMAGE_BASE, AREA_DAMAGE_SIZE);
		case SpellId::TRAP:
			return std::make_unique<TrapSpell>(TRAP_DAMAGE_BASE);
		case SpellId::SUMMON:
			return std::make_unique<SummonSpell>(SUMMON_HEALTH_BASE, SUMMON_DAMAGE_BASE);
		case SpellId::ENHANCEMENT:
		default:
			return std::make_unique<EnhancementSpell>();
	}
}

std::unique_ptr<Spell> SpellFactory::createUpgraded(SpellId id) {
	switch (id) {
		case SpellId::DIRECT_DAMAGE:
			return std::make_unique<DirectDamageSpell>(DIRECT_DAMAGE_UPGRADED, DIRECT_DAMAGE_UPGRADED_RADIUS);
		case SpellId::AREA_DAMAGE:
			return std::make_unique<AreaDamageSpell>(AREA_DAMAGE_UPGRADED, AREA_DAMAGE_UPGRADED_SIZE);
		case SpellId::TRAP:
			return std::make_unique<TrapSpell>(TRAP_DAMAGE_UPGRADED);
		case SpellId::SUMMON:
			return std::make_unique<SummonSpell>(SUMMON_HEALTH_UPGRADED, SUMMON_DAMAGE_UPGRADED);
		case SpellId::ENHANCEMENT:
		default:
			return std::make_unique<EnhancementSpell>();
	}
}

SpellId SpellFactory::randomId() {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, 4);
	return static_cast<SpellId>(dis(gen));
}

std::unique_ptr<Spell> SpellFactory::createRandomBasic() {
	return create(randomId());
}

