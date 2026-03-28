#ifndef HAND
#define HAND

#include "Spells.h"
#include "Constants.h"

class Hand
{
	int handCapacity;
	std::vector<std::shared_ptr<ISpell>> spells{};
public:
	Hand(int = STANDART_HAND_CAP, std::shared_ptr<ISpell> = nullptr);
	bool AddSpell(std::shared_ptr<ISpell>);
	bool CastSpell(int, std::pair<int, int>, std::pair<int, int>);
	std::shared_ptr<ISpell> GetSpell(int)const;
	int GetCurrSize()const;
	int GetCapacity()const { return handCapacity; }
};

#endif
