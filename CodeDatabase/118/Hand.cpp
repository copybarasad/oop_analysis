#include "Hand.h"

Hand::Hand(int maxCap, std::shared_ptr<ISpell> startSpell)
	:handCapacity(maxCap)
{
	if (startSpell != nullptr)
		spells.push_back(startSpell);
}

bool Hand::AddSpell(std::shared_ptr<ISpell> newSpell)
{
	if (spells.size() < handCapacity)
	{
		spells.push_back(newSpell);
		return true;
	}

	return false;
}

bool Hand::CastSpell(int numberOfSpell, std::pair<int, int> casterPos, std::pair<int, int> targetPos)
{
	if (numberOfSpell < spells.size() && spells[numberOfSpell]->Action(casterPos, targetPos))
	{
		for (auto i = numberOfSpell; i < spells.size() - 1; ++i)
			std::swap(spells[i], spells[i + 1]);

		spells.pop_back();
		return true;
	}

	return false;
}

std::shared_ptr<ISpell> Hand::GetSpell(int number) const{
	return number < spells.size() ? spells[number] : nullptr;
}

int Hand::GetCurrSize() const{
	return (int)spells.size();
}