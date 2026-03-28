#ifndef ENHACEMENTSPELL_H
#define ENHACEMENTSPELL_H

#include "ISpell.h"


class EnhacementSpell : public ISpell {
public:
	EnhacementSpell();
	EnhacementSpell(int);

	bool use(const SpellContext&) override;
	SpellType getSpellType() const override;
	void applyEnhancement(std::shared_ptr<Hand>);
	void upgrade(int) override;
	int getEnhancement();
	TokenSpell serialise() const override;

private:
	int enhancement_;
};


#endif //ENHACEMENTSPELL_H
