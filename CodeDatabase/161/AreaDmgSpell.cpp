#include "AreaDmgSpell.h"
#include "../../map/Field.h"

AreaDmgSpell::AreaDmgSpell() : AreaDmgSpell(2, 4) {};
AreaDmgSpell::AreaDmgSpell(int area, int damage) :
				area_(std::max(2, area)),
				damage_(std::max(2, damage)) {};



void AreaDmgSpell::upgrade(int coeficient){
	area_ *= coeficient;
}
bool AreaDmgSpell::use(const SpellContext& context) {
	if (!context.field) {
		return false;
	}

	for (int i = context.base_x - area_; i <= context.base_x + area_; i++) {
		for (int j = context.base_y - area_; j <= context.base_y + area_; j++) {
			auto entity = context.field->GetEntity(i, j);

			if (entity) {
				entity->CauseDamage(damage_);
			}
		}
	}

	return true;
}

int AreaDmgSpell::getArea() const {
	return area_;
}

int AreaDmgSpell::getDamage() const {
	return damage_;
}

SpellType AreaDmgSpell::getSpellType() const {
	return SpellType::AreaDmg;
}

TokenSpell AreaDmgSpell::serialise() const {
	TokenSpell token;
	token.type = "AreaDmg";
	token.damage = std::to_string(damage_);
	token.area = std::to_string(area_);
	return token;
}