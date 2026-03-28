#ifndef ENTITY_H
#define ENTITY_H

#include "../logic/Tokens.h"

class Entity {
public:
	Entity();
	int GetDamage() const;
	bool CauseDamage(int);
	bool IsDead() const;
	bool CanHit() const;
	bool NotMoveNext();
	bool IsStatic() const;
	void SetNotMoveNext(bool);
	int GetHP() const;
	virtual bool IsFriendly() const;
	virtual ~Entity() = default;

	virtual TokenEntity serialise() const = 0;

protected:
	int hp_;
	int damage_;
	bool can_move_on_cell_;
	bool can_hit_on_walking_;
	bool not_move_next_turn_;
};

#endif // ENTITY_H