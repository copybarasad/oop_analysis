#include "SummSpell.h"
#include "../../map/Field.h"
#include "../Ally.h"

SpellType SummSpell::getSpellType() const {
	return SpellType::Summon;
}

SummSpell::SummSpell() : SummSpell(1) {}

SummSpell::SummSpell(int count) {
	count_allies_ = (count > 0 && count < 3) ? count : 1;
}

int SummSpell::getAllyCount() const {
	return count_allies_;
}

void SummSpell::upgrade(int coeficient) {
	count_allies_+= coeficient;
}

bool SummSpell::use(const SpellContext& context) {
	if (!context.caster || !context.field || !context.hand) {
		return false;
	}

	int x_c, y_c;
	context.field->GetPosEntity(context.caster, x_c, y_c);

	int dx[4] = {-1, 1, 0, 0};
	int dy[4] = {0, 0, -1, 1};

	bool spawned = false;
	for (int j = 0; j < count_allies_; ++j) {
		for (int i = 0; i < 4; ++i) {
			int x = x_c + dx[i];
			int y = y_c + dy[i];

			auto ally = std::make_shared<Ally>();
			if (context.field->SetEntity(ally, x, y)) {
				spawned = true;
				break;
			}
		}
	}

	return spawned;
}

TokenSpell SummSpell::serialise() const {
	TokenSpell token;
	token.type = "Summon";
	token.count_allies = std::to_string(count_allies_);
	return token;
}