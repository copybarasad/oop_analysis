#ifndef ENEMYTOWER_H
#define ENEMYTOWER_H

#include "Entity.h"
#include "Spells/ISpell.h"

class EnemyTower : public Entity{
public:
	EnemyTower();
	EnemyTower(int, int);
	bool IsFriendly() const override;

	std::shared_ptr<ISpell> canAttack();
	TokenEntity serialise() const override;

private:
	std::shared_ptr<ISpell> spell_;

	int current_tick_ = 0;
	int base_tick_ = 2;
};

#endif //ENEMYTOWER_H
