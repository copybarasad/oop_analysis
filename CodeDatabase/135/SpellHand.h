#ifndef SPELL_HAND_H
#define SPELL_HAND_H

#include "ISpell.h"
#include <memory>
#include <vector>
#include <optional>

class SpellHand {
private:
	std::vector<std::unique_ptr<ISpell>> spells;
	size_t max_size;

public:
	explicit SpellHand(size_t hand_size);
	
	SpellHand(const SpellHand&) = delete;
	SpellHand& operator=(const SpellHand&) = delete;
	
	SpellHand(SpellHand&&) noexcept = default;
	SpellHand& operator=(SpellHand&&) noexcept = default;
	
	bool addSpell(std::unique_ptr<ISpell> spell);
	ISpell* getSpell(size_t index) const;
	std::unique_ptr<ISpell> useSpell(size_t index);
	
	bool hasSpace() const;
	size_t getCurrentSize() const;
	size_t getMaxSize() const;
	bool isEmpty() const;
	
	void increaseMaxSize(size_t amount);
	void removeHalfSpells();
};

#endif
