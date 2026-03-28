#ifndef DIRDAMAGESPELL_H
#define DIRDAMAGESPELL_H

#include "ISpell.h"

class DirDamageSpell : public ISpell {
public:
	DirDamageSpell();
	DirDamageSpell(int, int);

	void upgrade(int) override;
	bool use(const SpellContext&) override;

	int getDamage() const;
	int getRange() const;

	SpellType getSpellType() const override;
	TokenSpell serialise() const override;
private:
	int damage_;
	int range_;
};

#endif //DIRDAMAGESPELL_H