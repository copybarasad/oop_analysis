#ifndef SPELL_HAND_H
#define SPELL_HAND_H

#include <vector>
#include <memory>
#include "Spell.h"

class SpellHand {
private:
	std::vector<std::unique_ptr<Spell>> spells;
	int maxSize;

public:
	explicit SpellHand(int size, bool withStarter = true);

	// Запрет копирования
	SpellHand(const SpellHand&) = delete;
	SpellHand& operator=(const SpellHand&) = delete;

	// Разрешение перемещения
	SpellHand(SpellHand&&) = default;
	SpellHand& operator=(SpellHand&&) = default;

	bool addSpell(std::unique_ptr<Spell> spell);
	bool removeSpell(int index);
	bool replaceSpell(int index, std::unique_ptr<Spell> spell);
	Spell* getSpell(int index) const;
	int getSize() const;
	int getMaxSize() const;
	void increaseCapacity(int delta);
	void discardHalfRandomly();
	std::vector<SpellId> getSpellIds() const;
	void replaceSpells(std::vector<std::unique_ptr<Spell>>&& newSpells);
	void clear();
	void display() const;
};

#endif