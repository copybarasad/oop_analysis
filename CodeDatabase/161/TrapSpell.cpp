#include "TrapSpell.h"
#include "../../map/Field.h"

TrapSpell::TrapSpell() : TrapSpell(10) {};

TrapSpell::TrapSpell(int damage) : damage_(std::max(1, damage)) {};


void TrapSpell::upgrade(int coeficient){
	damage_ *= coeficient;
}


bool TrapSpell::use(const SpellContext& context) {
	if (!context.cell || !context.field) {
		return false;
	}

	int caster_x, caster_y;
	context.field->GetPosEntity(context.caster, caster_x, caster_y);
	int res = (caster_x - context.base_x)*(caster_x - context.base_x) + (caster_y - context.base_y)*(caster_y - context.base_y);
	if (res >= 4) {
		return false;
	}

	placeTrap(context.cell);

	return true;
}

SpellType TrapSpell::getSpellType() const {
	return SpellType::Trap;
}

int TrapSpell::getTrapDamage() const{
	return damage_;
}

void TrapSpell::placeTrap(std::shared_ptr<Cell> cell){
	std::shared_ptr<TrapEvent> trap = std::make_shared<TrapEvent>(damage_);

	cell->SetEvent(trap);
}

TokenSpell TrapSpell::serialise() const {
	TokenSpell token;
	token.type = "Trap";
	token.damage = std::to_string(damage_);
	return token;
}