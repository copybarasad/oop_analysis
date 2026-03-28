#ifndef AREADMGSPELL_H
#define AREADMGSPELL_H

#include "ISpell.h"

class AreaDmgSpell : public ISpell {
public:
	AreaDmgSpell();
	AreaDmgSpell(int, int);

	void upgrade(int) override;
	bool use(const SpellContext&) override;

	int getArea() const;
	int getDamage() const;

	SpellType getSpellType() const override;
	TokenSpell serialise() const override;

private:
	int damage_;
	int area_;
};


#endif //AREADMGSPELL_H
