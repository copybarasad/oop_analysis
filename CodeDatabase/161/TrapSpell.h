#ifndef TRAPSPELL_H
#define TRAPSPELL_H

#include "ISpell.h"
#include "../../map/cells/Events/TrapEvent.h"


class TrapSpell : public ISpell {
public:
	TrapSpell();
	TrapSpell(int damage);

	void upgrade(int) override;
	bool use(const SpellContext&) override;

	SpellType getSpellType() const override;

	int getTrapDamage() const;
	void placeTrap(std::shared_ptr<Cell>);
	TokenSpell serialise() const override;

private:
	int damage_;
};



#endif //TRAPSPELL_H
