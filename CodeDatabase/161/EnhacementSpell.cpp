#include "EnhacementSpell.h"
#include "./Hand.h"

EnhacementSpell::EnhacementSpell() : EnhacementSpell(2) {};
EnhacementSpell::EnhacementSpell(int coef) : enhancement_(std::max(2, coef)) {};

SpellType EnhacementSpell::getSpellType() const  {
	return SpellType::Enhancement;
}

bool EnhacementSpell::use(const SpellContext& context){
	if (context.hand) {
		applyEnhancement(context.hand);
		return true;
	}

	return false;
}


void EnhacementSpell::applyEnhancement(std::shared_ptr<Hand> hand) {
	hand->setUpgradeCoef(enhancement_);
}

void EnhacementSpell::upgrade(int coeficient) {
	enhancement_ += coeficient;
}

int EnhacementSpell::getEnhancement()  {
	return enhancement_;
}

TokenSpell EnhacementSpell::serialise() const {
	TokenSpell token;
	token.type = "Enhancement";
	token.enhancement = std::to_string(enhancement_);
	return token;
}