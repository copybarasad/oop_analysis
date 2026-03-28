#ifndef SUMMSPELL_H
#define SUMMSPELL_H

#include "ISpell.h"

class SummSpell : public ISpell {
public:
	SummSpell();
	SummSpell(int);

	int getAllyCount() const;
	void upgrade(int) override;
	bool use(const SpellContext&) override;
	SpellType getSpellType() const override;
	TokenSpell serialise() const override;
private:
	int count_allies_;
};

#endif // SUMMSPELL_H