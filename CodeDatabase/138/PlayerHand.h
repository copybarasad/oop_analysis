#ifndef PLAYERHAND_H
#define PLAYERHAND_H

#include <vector>
#include <memory>
#include <string>

class Spell;
class GameField;

class PlayerHand {
public:
	PlayerHand(size_t maxSize);
	
	bool AddSpell(std::unique_ptr<Spell> spell);
	bool UseSpell(size_t index, GameField& field, int targetX, int targetY);
	size_t GetSpellCount() const;
	std::string GetSpellName(size_t index) const;
	Spell* GetSpellAt(size_t index);
	void RemoveSpell(size_t index);
	bool IsFull() const;

private:
	std::vector<std::unique_ptr<Spell>> spells;
	size_t maxHandSize;
	
	void validateInvariant() const;
};

#endif
