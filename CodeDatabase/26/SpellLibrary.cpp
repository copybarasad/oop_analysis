#include "SpellLibrary.hpp"

#include <algorithm>
#include <array>

#include "AreaDamageSpell.hpp"
#include "DirectDamageSpell.hpp"
#include "EmpowerSpell.hpp"
#include "SummonSpell.hpp"
#include "TrapSpell.hpp"

namespace {

int g_directBaseRange = 3;
constexpr int kAreaBaseSize = 2;
int g_areaBaseRange = 4;
constexpr int kTrapBaseRange = 4;
constexpr int kTrapBaseLifetime = 5;

int g_directBaseDamage = 10;
int g_areaBaseDamage = 9;
int g_trapBaseDamage = 9;

int g_summonHp = 12;
int g_summonDamage = 3;
int g_summonCount = 1;

struct SpellFactoryEntry {
	int hotkey;
	std::unique_ptr<SpellBase> (*maker)();
};

std::unique_ptr<SpellBase> makeDirect() {
	return std::unique_ptr<SpellBase>(new DirectDamageSpell(g_directBaseDamage, g_directBaseRange));
}

std::unique_ptr<SpellBase> makeArea() {
	return std::unique_ptr<SpellBase>(new AreaDamageSpell(g_areaBaseDamage, kAreaBaseSize, g_areaBaseRange));
}

std::unique_ptr<SpellBase> makeTrap() {
	return std::unique_ptr<SpellBase>(new TrapSpell(g_trapBaseDamage, kTrapBaseRange, kTrapBaseLifetime));
}

std::unique_ptr<SpellBase> makeSummon() {
	return std::unique_ptr<SpellBase>(new SummonSpell(g_summonHp, g_summonDamage, g_summonCount));
}

std::unique_ptr<SpellBase> makeEmpower() {
	return std::unique_ptr<SpellBase>(new EmpowerSpell(1));
}

const std::array<SpellFactoryEntry, 5> kFactories = {
	SpellFactoryEntry{1, &makeDirect},
	SpellFactoryEntry{2, &makeArea},
	SpellFactoryEntry{3, &makeTrap},
	SpellFactoryEntry{4, &makeSummon},
	SpellFactoryEntry{5, &makeEmpower}
};

} // namespace

std::unique_ptr<SpellBase> SpellLibrary::createSpellByHotkey(int hotkey) {
	for (const auto& entry : kFactories) {
		if (entry.hotkey == hotkey) {
			return entry.maker();
		}
	}
	return nullptr;
}

std::vector<int> SpellLibrary::availableHotkeys() {
	std::vector<int> result;
	result.reserve(kFactories.size());
	for (const auto& entry : kFactories) {
		result.push_back(entry.hotkey);
	}
	return result;
}

void SpellLibrary::configureSummonStats(int hp, int damage, int count) {
	g_summonHp = std::max(1, hp);
	g_summonDamage = std::max(0, damage);
	g_summonCount = std::max(1, count);
}

void SpellLibrary::configureDirectRange(int range) {
	g_directBaseRange = std::max(1, range);
}

void SpellLibrary::configureAreaRange(int range) {
	g_areaBaseRange = std::max(1, range);
}

int SpellLibrary::directBaseDamage() noexcept {
	return g_directBaseDamage;
}

int SpellLibrary::directBaseRange() noexcept {
	return g_directBaseRange;
}

int SpellLibrary::areaBaseDamage() noexcept {
	return g_areaBaseDamage;
}

int SpellLibrary::areaBaseSize() noexcept {
	return kAreaBaseSize;
}

int SpellLibrary::areaBaseRange() noexcept {
	return g_areaBaseRange;
}

int SpellLibrary::trapBaseDamage() noexcept {
	return g_trapBaseDamage;
}

int SpellLibrary::trapBaseRange() noexcept {
	return kTrapBaseRange;
}

int SpellLibrary::trapBaseLifetime() noexcept {
	return kTrapBaseLifetime;
}

SpellLibrary::SummonStats SpellLibrary::summonStats() noexcept {
	return SummonStats{g_summonHp, g_summonDamage, g_summonCount};
}
