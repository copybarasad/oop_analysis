#pragma once

#include <memory>
#include <vector>

class SpellBase;

namespace SpellLibrary {

struct SummonStats {
	int hp;
	int damage;
	int count;
};

std::unique_ptr<SpellBase> createSpellByHotkey(int hotkey);
std::vector<int> availableHotkeys();

void configureSummonStats(int hp, int damage, int count);
void configureDirectRange(int range);
void configureAreaRange(int range);

int directBaseDamage() noexcept;
int directBaseRange() noexcept;

int areaBaseDamage() noexcept;
int areaBaseSize() noexcept;
int areaBaseRange() noexcept;

int trapBaseDamage() noexcept;
int trapBaseRange() noexcept;
int trapBaseLifetime() noexcept;

SummonStats summonStats() noexcept;

} // namespace SpellLibrary
