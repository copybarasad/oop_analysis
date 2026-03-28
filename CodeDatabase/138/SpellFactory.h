#ifndef SPELLFACTORY_H
#define SPELLFACTORY_H

#include <memory>
#include "Spell.h"

class SpellFactory {
public:
	static std::unique_ptr<Spell> CreateRandomSpell(int fieldWidth, int fieldHeight);
	static std::unique_ptr<Spell> CreateDirectDamageSpell(int fieldWidth, int fieldHeight);
	static std::unique_ptr<Spell> CreateAreaDamageSpell(int fieldWidth, int fieldHeight);
	static std::unique_ptr<Spell> CreateTrapSpell(int fieldWidth, int fieldHeight);

private:
	static int calculateRadius(int fieldSize);
};

#endif
